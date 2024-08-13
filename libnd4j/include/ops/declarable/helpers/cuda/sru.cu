/* ******************************************************************************
 *
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 *  See the NOTICE file distributed with this work for additional
 *  information regarding copyright ownership.
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//
// implementation of operations for Simple Recurrent Unit: arXiv:1709.02755v2 [cs.CL] 12 Sep 2017
//
//  @author Yurii Shyrma, created on 05.12.2017
//
#include <array/NDArrayFactory.h>
#include <helpers/MmulHelper.h>
#include <helpers/PointersManager.h>
#include <ops/declarable/helpers/sru.h>

#include "execution/cuda/LaunchDims.h"


namespace sd {
namespace ops {
namespace helpers {

//////////////////////////////////////////////////////////////////////////
static SD_INLINE NDArray activation(const NDArray& arr) {
  // return (const_cast<NDArray<T>&>(arr)).template transform<simdOps::Tanh<T>>();
  auto result = NDArray(&arr, false, arr.getContext());
  (const_cast<NDArray&>(arr)).applyTransform(transform::Tanh, result);
  return result;
}

//////////////////////////////////////////////////////////////////////////
static SD_INLINE NDArray sigmoid(const NDArray& arr) {
  return (const_cast<NDArray&>(arr)).transform(transform::Sigmoid);
}

//////////////////////////////////////////////////////////////////////////
void sruCell(LaunchContext* context, const NDArray* x, const NDArray* c0, const NDArray* w, const NDArray* b,
             NDArray* h, NDArray* c) {
  // x   input [bS x inSize], bS - batch size, inSize - number of features
  // c0  previous cell state c  [bS x inSize], that is at previous time step t-1
  // w   weights [inSize x 3*inSize]
  // b   biases [2*inSize]

  // h   current cell output [bS x inSize], that is at current time step t
  // c   current cell state  [bS x inSize], that is at current time step t

  const int inSize = x->sizeAt(1);  // inSize - number of features

  auto z = mmul(*x, *w);  //  [bS x 3*inSize]

  // forget gate = sigmoid(x*Wf + bf)
  auto f = sigmoid(z({0, 0, inSize, 2 * inSize}) + (*b)({0, inSize}));

  // reset gate = sigmoid(x*Wr + br)
  auto r = sigmoid(z({0, 0, 2 * inSize, 3 * inSize}) + (*b)({inSize, 2 * inSize}));

  // ◦ means element-wise product or so called Hadamard product
  // current sell state = f◦c0 + (1 - f)◦(x*Wc)
  c->assign(f * (*c0) + (1.f - f) * z({0, 0, 0, inSize}));
  // *c = f*(*c0 - z({},{0, inSize})) + z({{},{0, inSize}});

  // current cell output = r◦activation(c) + (1 - r)◦x
  h->assign(r * activation(*c) + (1.f - r) * (*x));
  // *h = r * (activation<T>(c) - *x) + *x;
}

//////////////////////////////////////////////////////////////////////////
void sruTimeLoop(LaunchContext* context, const NDArray* x, const NDArray* c0, const NDArray* w, const NDArray* b,
                 NDArray* h, NDArray* c) {
  // x   input [bS x inSize x time]
  // c0  initial cell state  (at time step = 0) [bS x inSize],
  // w   weights, [3*inSize x inSize]
  // b   biases,  [2*inSize]

  // h   cell outputs [bS x inSize x time]
  // c   cell states  [bS x inSize x time]

  auto wT = w->transpose();  // [3*inSize x inSize] -> [inSize x 3*inSize]

  const int time = x->sizeAt(2);

  NDArray ct_1(*c0);

  // loop through time steps
  for (int t = 0; t < time; ++t) {
    auto xt = (*x)({0, 0, 0, 0, t, t + 1});
    auto ht = (*h)({0, 0, 0, 0, t, t + 1});
    auto ct = (*c)({0, 0, 0, 0, t, t + 1});

    sruCell(context, &xt, &ct_1, &wT, b, &ht, &ct);
    ct_1.assign(ct);
  }
}

//////////////////////////////////////////////////////////////////////////
template <typename T>
SD_KERNEL static void sruBICuda(const void* vx, const LongType* xShapeInfo, const void* vwi,
                                const LongType* wiShapeInfo, const void* vb, const LongType* bShapeInfo,
                                const void* vc0, const LongType* c0ShapeInfo, const void* vmask,
                                const LongType* maskShapeInfo, void* vht, const LongType* htShapeInfo,
                                void* vct,
                                const LongType* ctShapeInfo) {
  // inputs:
  // x     [time, bS, 2*K]
  // wi    [time, bS, 6*K], wi = mmul(x, weights);
  // b     [4*K]
  // c0    [bS, 2*K]
  // mask  [bS, 2*K], optional

  // outputs
  // ht  [time, bS, 2*K]
  // ct  [time, bS, 2*K]

  const auto x = reinterpret_cast<const T*>(vx);
  const auto wi = reinterpret_cast<const T*>(vwi);
  const auto b = reinterpret_cast<const T*>(vb);
  const auto c0 = reinterpret_cast<const T*>(vc0);
  const auto mask = reinterpret_cast<const T*>(vmask);
  auto ht = reinterpret_cast<T*>(vht);
  auto ct = reinterpret_cast<T*>(vct);

  const LongType rank = 3;

  __shared__ LongType time, K, *sharedMem;
  __shared__ LongType len, totalThreads;

  if (threadIdx.x == 0) {
    extern __shared__ unsigned char shmem[];
    sharedMem = reinterpret_cast<LongType*>(shmem);

    time = xShapeInfo[1];
    K = xShapeInfo[3] / 2;
    len = xShapeInfo[2] * xShapeInfo[3];  // 2*K*bS

    totalThreads = gridDim.x * blockDim.x;
  }
  __syncthreads();

  const LongType tid = blockIdx.x * blockDim.x + threadIdx.x;
  LongType *coords = sharedMem + threadIdx.x * rank;

  if (tid >= len) return;

  shape::index2coords(tid, rank - 1, xShapeInfo + 2, coords + 1);  // loop through last two dimensions of x : {bS, 2*K}

  const auto maskOffst = mask ? shape::getOffset(maskShapeInfo, coords + 1) : 0;
  const auto c0Offset = shape::getOffset(c0ShapeInfo, coords + 1);
  const auto bFOffset = shape::getOffset(bShapeInfo, coords + 2);
  const auto bROffset = bFOffset + 2 * K * bShapeInfo[2];  // 2*K*b_stride

  const T maskVal = mask ? mask[maskOffst] : static_cast<T>(1);
  const T bF = b[bFOffset];
  const T bR = b[bROffset];
  T c0Val = c0[c0Offset];

  const bool flip = coords[2] >= K;

  if (flip)
    coords[0] = time - 1;
  else
    coords[0] = 0;

  auto xOffset = shape::getOffset(xShapeInfo, coords);
  auto htOffset = shape::getOffset(htShapeInfo, coords);
  auto ctOffset = shape::getOffset(ctShapeInfo, coords);

  coords[2] *= 3;
  auto wiOffset0 = shape::getOffset(wiShapeInfo, coords);
  auto wiOffset1 = wiOffset0 + wiShapeInfo[rank + 3];  // add last stride
  auto wiOffset2 = wiOffset1 + wiShapeInfo[rank + 3];  // add last stride

  // time loop
  for (LongType t = 0; t < time; ++t) {
    // evaluate sigmoids
    T ft = (1.f) / (1.f + math::sd_exp<T, T>(-(wi[wiOffset1] + bF)));
    T rt = (1.f) / (1.f + math::sd_exp<T, T>(-(wi[wiOffset2] + bR)));

    c0Val = (c0Val - wi[wiOffset0]) * ft + wi[wiOffset0];
    ct[ctOffset] = c0Val;
    T val = math::sd_tanh<T, T>(c0Val);
    T xVal = x[xOffset];
    ht[htOffset] = (val * maskVal - xVal) * rt + xVal;

    if (flip) {
      xOffset -= xShapeInfo[rank + 1];  // first stride, corresponds to time step
      htOffset -= htShapeInfo[rank + 1];
      ctOffset -= htShapeInfo[rank + 1];
      wiOffset0 -= wiShapeInfo[rank + 1];
      wiOffset1 -= wiShapeInfo[rank + 1];
      wiOffset2 -= wiShapeInfo[rank + 1];
    } else {
      xOffset += xShapeInfo[rank + 1];  // first stride, corresponds to time step
      htOffset += htShapeInfo[rank + 1];
      ctOffset += htShapeInfo[rank + 1];
      wiOffset0 += wiShapeInfo[rank + 1];
      wiOffset1 += wiShapeInfo[rank + 1];
      wiOffset2 += wiShapeInfo[rank + 1];
    }
  }
}

//////////////////////////////////////////////////////////////////////////
template <typename T>
static void sruBICudaLauncher(const int blocksPerGrid, const int threadsPerBlock, const int sharedMem,
                              const cudaStream_t* stream, const void* vx, const LongType* xShapeInfo,
                              const void* vwi,
                              const LongType* wiShapeInfo, const void* vb, const LongType* bShapeInfo, const void* vc0,
                              const LongType* c0ShapeInfo,
                              const void* vmask, const LongType* maskShapeInfo, void* vht,
                              const LongType* htShapeInfo, void* vct, const LongType* ctShapeInfo) {
  sruBICuda<T><<<blocksPerGrid, threadsPerBlock, sharedMem, *stream>>>(vx, xShapeInfo, vwi, wiShapeInfo, vb, bShapeInfo,
                                                                       vc0, c0ShapeInfo, vmask, maskShapeInfo, vht,
                                                                       htShapeInfo, vct, ctShapeInfo);
  sd::DebugHelper::checkErrorCode(const_cast<cudaStream_t *>(stream), "sruBICuda failed");

}

//////////////////////////////////////////////////////////////////////////
void sruBI(LaunchContext* context, NDArray* x, const NDArray* w, const NDArray* b, const NDArray* c0,
           const NDArray* mask, NDArray* ht, NDArray* ct) {
  //  x = x * mask
  std::vector<LongType> dims = {1,2};
  if (mask) x->applyBroadcast(broadcast::Multiply, &dims, *mask, *x);  // apply mask

  // U = x * w
  NDArray wi = mmul(*x, *w);  //  U [time x bS x 6*K]

  PointersManager manager(context, "sru_bi");

  dim3 sruBiDims2 = sruBiDims(x->sizeAt(1) * x->sizeAt(2),x->rankOf());
  NDArray::prepareSpecialUse({ht, ct}, {x, &wi, b, c0, mask});
  BUILD_SINGLE_SELECTOR(
      x->dataType(), sruBICudaLauncher,
      (sruBiDims2.y,sruBiDims2.x, sruBiDims2.z, context->getCudaStream(), x->specialBuffer(), x->specialShapeInfo(),
          wi.specialBuffer(), wi.specialShapeInfo(), b->specialBuffer(), b->specialShapeInfo(), c0->specialBuffer(),
          c0->specialShapeInfo(), mask ? mask->specialBuffer() : nullptr, mask ? mask->specialShapeInfo() : nullptr,
          ht->specialBuffer(), ht->specialShapeInfo(), ct->specialBuffer(), ct->specialShapeInfo()),
      SD_FLOAT_TYPES);
  NDArray::registerSpecialUse({ht, ct}, {x, &wi, b, c0, mask});

  manager.synchronize();
}

//////////////////////////////////////////////////////////////////////////
template <typename T>
SD_KERNEL static void sruBIBPCuda(const void* vx, const LongType* xShapeInfo, const void* vwi,
                                  const LongType* wiShapeInfo, const void* vb, const LongType* bShapeInfo,
                                  const void* vc0, const LongType* c0ShapeInfo, const void* vmask,
                                  const LongType* maskShapeInfo, const void* vct, const LongType* ctShapeInfo,
                                  const void* vgradHt, const LongType* gradHtShapeInfo, const void* vgradCt,
                                  const LongType* gradCtShapeInfo, void* vgradI, const LongType* gradIShapeInfo,
                                  void* vgradWi, const LongType* gradWiShapeInfo, void* vgradB,
                                  const LongType* gradBShapeInfo, void* vgradC0, const LongType* gradC0ShapeInfo) {
  // inputs:
  // x      [time, bS, 2*K]
  // wi     [time, bS, 6*K], wi = mmul(x, weights);
  // b      [4*K]
  // c0     [bS, 2*K]
  // mask   [bS, 2*K], optional
  // ct     [time, bS, 2*K]
  // gradHt [time, bS, 2*K]
  // gradCt [bS, 2*K]

  // outputs
  // gradI   [time, bS, 2*K]
  // gradWi  [time, 2*K, 6*K]
  // gradB   [bS, 4*K]
  // gradC0  [bS, 2*K]

  const auto x = reinterpret_cast<const T*>(vx);
  const auto wi = reinterpret_cast<const T*>(vwi);
  const auto b = reinterpret_cast<const T*>(vb);
  const auto c0 = reinterpret_cast<const T*>(vc0);
  const auto mask = reinterpret_cast<const T*>(vmask);
  const auto ct = reinterpret_cast<const T*>(vct);
  const auto gradHt = reinterpret_cast<const T*>(vgradHt);
  const auto gradCt = reinterpret_cast<const T*>(vgradCt);

  auto gradI = reinterpret_cast<T*>(vgradI);
  auto gradWi = reinterpret_cast<T*>(vgradWi);
  auto gradB = reinterpret_cast<T*>(vgradB);
  auto gradC0 = reinterpret_cast<T*>(vgradC0);

  const int rank = 3;

  __shared__ LongType time, K, *sharedMem;
  __shared__ LongType len, totalThreads;

  if (threadIdx.x == 0) {
    extern __shared__ unsigned char shmem[];
    sharedMem = reinterpret_cast<LongType*>(shmem);

    time = xShapeInfo[1];
    K = xShapeInfo[3] / 2;
    len = xShapeInfo[2] * xShapeInfo[3];  // 2*K*bS

    totalThreads = gridDim.x * blockDim.x;
  }

  __syncthreads();

  const LongType tid = blockIdx.x * blockDim.x + threadIdx.x;
  auto coords = sharedMem + threadIdx.x * rank;

  if (tid >= len) return;

  shape::index2coords(tid, rank - 1, xShapeInfo + 2, coords + 1);  // loop through last two dimensions of x : {bS, 2*K}

  const auto maskOffst = mask ? shape::getOffset(maskShapeInfo, coords + 1) : 0;
  const auto c0Offset = shape::getOffset(c0ShapeInfo, coords + 1);
  const auto gradCtOffset = shape::getOffset(gradCtShapeInfo, coords + 1);
  const auto gradC0Offset = shape::getOffset(gradC0ShapeInfo, coords + 1);
  const auto bFOffset = shape::getOffset(bShapeInfo, coords + 2);
  const auto bROffset = bFOffset + 2 * K * bShapeInfo[2];  // 2*K*b_stride
  const auto gradBFOffset = coords[1] * gradBShapeInfo[3] / 2 + coords[2] * gradBShapeInfo[4];
  const auto gradBROffset = gradBFOffset + gradBShapeInfo[3];

  const bool flip = coords[2] >= K;

  if (flip)
    coords[0] = 0;
  else
    coords[0] = time - 1;

  auto xOffset = shape::getOffset(xShapeInfo, coords);
  auto ctOffset = shape::getOffset(ctShapeInfo, coords);
  auto gradIOffset = shape::getOffset(gradIShapeInfo, coords);
  auto gradHtOffset = shape::getOffset(gradHtShapeInfo, coords);

  coords[2] *= 3;
  auto gradWiOffset0 = shape::getOffset(gradWiShapeInfo, coords);
  auto gradWiOffset1 = gradWiOffset0 + gradWiShapeInfo[rank + 3];  // add last stride
  auto gradWiOffset2 = gradWiOffset1 + gradWiShapeInfo[rank + 3];  // add last stride
  auto wiOffset0 = shape::getOffset(wiShapeInfo, coords);
  auto wiOffset1 = wiOffset0 + wiShapeInfo[rank + 3];  // add last stride
  auto wiOffset2 = wiOffset1 + wiShapeInfo[rank + 3];  // add last stride

  const T xVal = x[xOffset];
  const T maskVal = mask ? mask[maskOffst] : static_cast<T>(1);
  const T c0Val = c0[c0Offset];
  const T bF = b[bFOffset];
  const T bR = b[bROffset];
  T gradCtVal = gradCt[gradCtOffset];
  T gbF = 0.f;
  T gbR = 0.f;

  // time loop
  for (LongType t = 0; t < time; ++t) {
    // evaluate sigmoids
    T ft = (1.f) / (1.f + math::sd_exp<T, T>(-(wi[wiOffset1] + bF)));
    T rt = (1.f) / (1.f + math::sd_exp<T, T>(-(wi[wiOffset2] + bR)));

    T val = math::sd_tanh<T, T>(ct[ctOffset]);

    T prevVal;
    if (t < time - 1)
      prevVal = ct[ctOffset += flip ? ctShapeInfo[rank + 1] : -ctShapeInfo[rank + 1]];
    else
      prevVal = c0Val;

    // grad wrt input
    gradI[gradIOffset] = gradHt[gradHtOffset] - gradHt[gradHtOffset] * rt;

    // grad wrt rt, wiR and bR
    T grt = gradHt[gradHtOffset] * (val * maskVal - x[xOffset]) * (rt - rt * rt);
    gradWi[gradWiOffset2] = grt;
    gbR += grt;

    // grad wrt state
    T gradC0Val = gradHt[gradHtOffset] * maskVal * (rt - rt * val * val) + gradCtVal;

    // grad wrt wi0
    gradWi[gradWiOffset0] = gradC0Val - gradC0Val * ft;

    // grad wrt ft, wi1, and bF
    T gft = gradC0Val * (prevVal - wi[wiOffset0]) * (ft - ft * ft);
    gradWi[gradWiOffset1] = gft;
    gbF += gft;

    // grad wrt c_previous
    gradCtVal = gradC0Val * ft;

    if (flip) {
      xOffset += xShapeInfo[rank + 1];  // first stride, corresponds to time step
      gradHtOffset += gradHtShapeInfo[rank + 1];
      gradIOffset += gradIShapeInfo[rank + 1];
      wiOffset0 += wiShapeInfo[rank + 1];
      wiOffset1 += wiShapeInfo[rank + 1];
      wiOffset2 += wiShapeInfo[rank + 1];
      gradWiOffset0 += gradWiShapeInfo[rank + 1];
      gradWiOffset1 += gradWiShapeInfo[rank + 1];
      gradWiOffset2 += gradWiShapeInfo[rank + 1];
    } else {
      xOffset -= xShapeInfo[rank + 1];  // first stride, corresponds to time step
      gradHtOffset -= gradHtShapeInfo[rank + 1];
      gradIOffset -= gradIShapeInfo[rank + 1];
      wiOffset0 -= wiShapeInfo[rank + 1];
      wiOffset1 -= wiShapeInfo[rank + 1];
      wiOffset2 -= wiShapeInfo[rank + 1];
      gradWiOffset0 -= gradWiShapeInfo[rank + 1];
      gradWiOffset1 -= gradWiShapeInfo[rank + 1];
      gradWiOffset2 -= gradWiShapeInfo[rank + 1];
    }
  }

  gradB[gradBFOffset] = gbF;
  gradB[gradBROffset] = gbR;
  gradC0[gradC0Offset] = gradCtVal;
}

//////////////////////////////////////////////////////////////////////////
template <typename T>
static void sruBIBPCudaLauncher(
    const int blocksPerGrid, const int threadsPerBlock, const int sharedMem, const cudaStream_t* stream, const void* vx, const LongType* xShapeInfo, const void* vwi,
    const LongType* wiShapeInfo, const void* vb, const LongType* bShapeInfo, const void* vc0, const LongType* c0ShapeInfo, const void* vmask,
    const LongType* maskShapeInfo, const void* vct, const LongType* ctShapeInfo, const void* vgradHt, const LongType* gradHtShapeInfo, const void* vgradCt,
    const LongType* gradCtShapeInfo, void* vgradI, const LongType* gradIShapeInfo, void* vgradWi, const LongType* gradWiShapeInfo, void* vgradB,
    const LongType* gradBShapeInfo, void* vgradC0, const LongType* gradC0ShapeInfo) {
  sruBIBPCuda<T><<<blocksPerGrid, threadsPerBlock, sharedMem, *stream>>>(
      vx, xShapeInfo, vwi, wiShapeInfo, vb, bShapeInfo, vc0, c0ShapeInfo, vmask, maskShapeInfo, vct, ctShapeInfo,
      vgradHt, gradHtShapeInfo, vgradCt, gradCtShapeInfo, vgradI, gradIShapeInfo, vgradWi, gradWiShapeInfo, vgradB,
      gradBShapeInfo, vgradC0, gradC0ShapeInfo);
  sd::DebugHelper::checkErrorCode(const_cast<cudaStream_t *>(stream), "sruBIBPCuda failed");

}
BUILD_SINGLE_TEMPLATE(template void sruBIBPCudaLauncher,
                      (const int blocksPerGrid, const int threadsPerBlock, const int sharedMem,
                          const cudaStream_t* stream, const void* vx, const sd::LongType* xShapeInfo, const void* vwi,
                          const sd::LongType* wiShapeInfo, const void* vb, const sd::LongType* bShapeInfo, const void* vc0,
                          const sd::LongType* c0ShapeInfo, const void* vmask, const sd::LongType* maskShapeInfo,
                          const void* vct, const sd::LongType* ctShapeInfo, const void* vgradHt,
                          const sd::LongType* gradHtShapeInfo, const void* vgradCt, const sd::LongType* gradCtShapeInfo,
                          void* vgradI, const sd::LongType* gradIShapeInfo, void* vgradWi,
                          const sd::LongType* gradWiShapeInfo, void* vgradB, const sd::LongType* gradBShapeInfo,
                          void* vgradC0, const sd::LongType* gradC0ShapeInfo),
                      SD_FLOAT_TYPES);

//////////////////////////////////////////////////////////////////////////
void sruBIBP(LaunchContext* context, NDArray* x, const NDArray* w, const NDArray* b, const NDArray* c0,
             const NDArray* ct, const NDArray* gradCt, const NDArray* gradHt, const NDArray* mask, NDArray* gradI,
             NDArray* gradW, NDArray* gradB, NDArray* gradC0) {
  //  x = x * mask
  std::vector<LongType> dims = {1, 2};
  if (mask) x->applyBroadcast(broadcast::Multiply, &dims, *mask, *x);  // apply mask

  // U = x * w
  NDArray wi = mmul(*x, *w);  //  U [time x bS x 6*K]

  const int time = x->sizeAt(0);
  const int bS = x->sizeAt(1);
  const int K = x->sizeAt(2) / 2;

  std::vector<sd::LongType> gradBiasShape = {bS, 4 * K};
  std::vector<sd::LongType> gradWiShape = {time, bS, 6 * K};
  NDArray gradBias(x->ordering(), gradBiasShape, x->dataType(), context);
  NDArray gradWi(x->ordering(), gradWiShape, x->dataType(), context);

  PointersManager manager(context, "sru_bi_bp");

  const int threadsPerBlock = SD_MAX_NUM_THREADS / 4;
  const int blocksPerGrid = (x->sizeAt(1) * x->sizeAt(2) + threadsPerBlock - 1) /
                            threadsPerBlock;  // loop through last two dimensions of x array -> bS, 2*K
  const int sharedMem = threadsPerBlock * sizeof(LongType) * x->rankOf() + 128;
  dim3 sruBiBpDims = sruBiDims(x->sizeAt(1) + x->sizeAt(2),x->rankOf());
  NDArray::prepareSpecialUse({gradI, &gradWi, &gradBias, gradC0}, {x, &wi, b, c0, ct, gradCt, gradHt, mask});
  BUILD_SINGLE_SELECTOR(
      x->dataType(), sruBIBPCudaLauncher,
      (sruBiBpDims.y, sruBiBpDims.x,sruBiBpDims.z, context->getCudaStream(), x->specialBuffer(), x->specialShapeInfo(),
          wi.specialBuffer(), wi.specialShapeInfo(), b->specialBuffer(), b->specialShapeInfo(), c0->specialBuffer(),
          c0->specialShapeInfo(), mask ? mask->specialBuffer() : nullptr, mask ? mask->specialShapeInfo() : nullptr,
          ct->specialBuffer(), ct->specialShapeInfo(), gradHt->specialBuffer(), gradHt->specialShapeInfo(),
          gradCt->specialBuffer(), gradCt->specialShapeInfo(), gradI->specialBuffer(), gradI->specialShapeInfo(),
          gradWi.specialBuffer(), gradWi.specialShapeInfo(), gradBias.specialBuffer(), gradBias.specialShapeInfo(),
          gradC0->specialBuffer(), gradC0->specialShapeInfo()),
      SD_FLOAT_TYPES);
  NDArray::registerSpecialUse({gradI, &gradWi, &gradBias, gradC0}, {x, &wi, b, c0, ct, gradCt, gradHt, mask});

  manager.synchronize();


  std::vector<LongType> dims2 = {0};
  // gradB
  gradBias.reduceAlongDimension(reduce::Sum, *gradB, &dims2);  // [4*K]

  // gradW
  x->permutei({0, 2, 1},false);                       // [time, bS, 2*K] -> [time, 2*K,  bS]
  MmulHelper::mmul(x, &gradWi, gradW, 1., 0.);  // [time, 2*K, bS] x [time, bS , 6*K] = [time, 2*K, 6*K]
}

}  // namespace helpers
}  // namespace ops
}  // namespace sd
