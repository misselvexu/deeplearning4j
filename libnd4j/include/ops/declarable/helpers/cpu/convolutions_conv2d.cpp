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
// @author Yurii Shyrma (iuriish@yahoo.com), created on 18.09.2018
//
#include <array/NDArrayFactory.h>
#include <execution/Threads.h>
#include <helpers/MmulHelper.h>
#include <ops/declarable/helpers/addBias.h>
#include <ops/declarable/helpers/convolutions.h>
#include <ops/declarable/helpers/im2col.h>

#if NOT_EXCLUDED(OP_col2im) && NOT_EXCLUDED(OP_im2col)

namespace sd {
namespace ops {

//////////////////////////////////////////////////////////////////////////
template <typename X, typename Y>
static void conv2d_(sd::graph::Context& block, NDArray* input, NDArray* weights, NDArray* bias,
                    NDArray* output, const LongType kH, const LongType kW, const LongType sH, const LongType sW, LongType pH, LongType pW,
                    const LongType dH, const LongType dW, const int paddingMode, const int isNCHW, const int wFormat) {

  // input   [bS, iH, iW, iC] (NHWC) or [bS, iC, iH, iW] (NCHW)
  // weights [kH, kW, iC, oC], [oC, iC, kH, kW], [oC, kH, kW, iC]
  // bias    [oC]
  // output  [bS, oH, oW, oC] (NHWC) or [bS, oC, oH, oW] (NCHW)

  LongType bS = input->sizeAt(0);
  LongType iC = ConvolutionUtils::inChannels(weights->shapeInfo(), wFormat);
  LongType oC = ConvolutionUtils::outChannels(weights->shapeInfo(), wFormat);
  LongType iH = ConvolutionUtils::inputHeight(input->shapeInfo(), isNCHW);
  LongType iW = ConvolutionUtils::inputWidth(input->shapeInfo(), isNCHW);
  LongType oH = ConvolutionUtils::calcOutDimConv(iH, kH, sH, pH, dH, paddingMode);
  LongType oW = ConvolutionUtils::calcOutDimConv(iW, kW, sW, pW, dW, paddingMode);

  std::vector<LongType> wAxes;
  if (0 == wFormat)
    wAxes = {0, 1, 2};
  else if (1 == wFormat)
    wAxes = {2, 3, 1};
  else
    wAxes = {1, 2, 3};


  NDArray *col = new NDArray('c', {bS, oH, oW, kH, kW, iC}, input->dataType(), input->getContext());
  NDArray *colP = new NDArray(col->permute({0, 3, 4, 5, 1, 2}, false));  // {bS, iC, kH, kW, oH, oW}
  NDArray mmulResult('f', {bS * oH * oW, oC}, output->dataType(), output->getContext());

  std::vector<LongType> permuteForOutput = {0, 3, 1, 2};

  //----- calculation of output -----//
  auto ctx = block.launchContext();


  if (isNCHW) {
    helpers::im2col(*ctx, *input, *colP, kH, kW, sH, sW, pH, pW, dH, dW,
                    NDArrayFactory::create(0.f, input->getContext()));
  } else {
    // For NHWC, we need to permute the input to NCHW before im2col
    NDArray* inputNchw = new NDArray(input->permute({0, 3, 1, 2}));
    helpers::im2col(*ctx, *inputNchw, *colP, kH, kW, sH, sW, pH, pW, dH, dW,
                    NDArrayFactory::create(0.f, input->getContext()));
  }



  std::vector<sd::LongType> permute = {0, 3, 4, 5, 1, 2};
  block.pushIntermediateResult(col);

  auto im2colReshape = col->reshape('c', {bS * oH * oW, kH * kW * iC}, true);

  auto weightsPermuted = weights->permute({3, 2, 1, 0});
  auto reshapedW = weightsPermuted.reshape('f', {iC * kH * kW, oC}, false);
  MmulHelper::matmul(&im2colReshape, &reshapedW, &mmulResult, false, false, 1.0, 0.0);


  auto reshaped = mmulResult.reshape('f', {oH,oW,bS,oC}, false);
  auto permuted = reshaped.permute({2,3,1,0});

  // Reshape and copy result to output
  if (isNCHW) {
    output->assign(permuted);
  } else {
    permuted = permuted.permute({0,2,3,1});
    output->assign(permuted);
  }

  //----- add biases if required -----//
  if (bias) {
    helpers::addBias(block, *output, *bias, *output, isNCHW);
  }

}
void ConvolutionUtils::conv2d(sd::graph::Context& block, NDArray* input, NDArray* weights,
                              NDArray* bias, NDArray* output, const LongType kH, const LongType kW, const LongType sH,
                              const LongType sW, LongType pH, LongType pW, const LongType dH, const LongType dW, const int paddingMode,
                              const int isNCHW, const int wFormat) {
  BUILD_SINGLE_SELECTOR_TWICE(
      input->dataType(), conv2d_,
      (block, input, weights, bias, output, kH, kW, sH, sW, pH, pW, dH, dW, paddingMode, isNCHW, wFormat),
      SD_FLOAT_TYPES);
}

}  // namespace ops
}  // namespace sd
#endif