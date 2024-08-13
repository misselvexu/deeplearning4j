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
// @author Yurii Shyrma (iuriish@yahoo.com), created on 20.04.2018
//

#include <helpers/Loops.h>
#include <helpers/ShapeUtils.h>
#include <ops/declarable/helpers/transforms.h>
#if NOT_EXCLUDED(OP_tile)
namespace sd {
namespace ops {
namespace helpers {

//////////////////////////////////////////////////////////////////////////
template <typename T>
static void tileBP_(const NDArray& gradO /*input*/, NDArray& gradI /*output*/, const std::vector<sd::LongType> reps) {
  T* gradIBuff = reinterpret_cast<T*>(gradI.buffer());
  auto gradOBuff = reinterpret_cast<T const*>(gradO.buffer());
  const sd::LongType gradILen = gradI.lengthOf();
  const sd::LongType gradOLen = gradO.lengthOf();  // gradOLen >= gradILen
  const sd::LongType gradIEWS = sd::math::sd_abs<sd::LongType>(gradI.ews());
  const sd::LongType gradOEWS = gradO.ews();

  // initial zeroing of gradI content
  if (gradIEWS == 1)
    memset(gradIBuff, 0, gradILen * sizeof(T));
  else {
    for (sd::LongType i = 0; i < gradILen * gradIEWS; i += gradIEWS) gradIBuff[i] = static_cast<T>(0.f);
  }

  if (gradO.ordering() == 'c' && gradOEWS == 1) {
    for (sd::LongType i = 0; i < gradOLen; ++i) {
      auto idx = shape::subArrayIndex(i, gradO.shapeInfo(), gradI.shapeInfo());
      gradI.p(idx, gradI.e<T>(idx) + gradOBuff[i]);
    }
  } else if (gradO.ordering() == 'c' && gradOEWS > 1) {
    for (sd::LongType i = 0; i < gradOLen; ++i) {
      sd::LongType idx = shape::subArrayIndex(i, gradO.shapeInfo(), gradI.shapeInfo());
      gradI.p(idx, gradI.e<T>(idx) + gradOBuff[i * gradOEWS]);
    }
  } else {
    for (sd::LongType i = 0; i < gradOLen; ++i) {
      sd::LongType fidx = shape::subArrayIndex(i, gradO.shapeInfo(), gradI.shapeInfo());
      sd::LongType  outIdx = shape::getIndexOffset(i, gradO.shapeInfo());
      gradI.p(fidx, gradI.e<T>(fidx) + gradOBuff[outIdx]);
    }
  }
}

void tileBP(LaunchContext* context, NDArray gradO /*input*/, NDArray& gradI /*output*/,
            const std::vector<LongType> reps) {
  BUILD_SINGLE_SELECTOR(gradI.dataType(), tileBP_, (gradO, gradI, reps), SD_FLOAT_TYPES);
}

BUILD_SINGLE_TEMPLATE(template void tileBP_,
                      (const NDArray& gradO /*input*/, NDArray& gradI /*output*/, const std::vector<sd::LongType> reps),
                      SD_FLOAT_TYPES);

}  // namespace helpers
}  // namespace ops
}  // namespace sd
#endif