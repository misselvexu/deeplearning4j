/*
 *  ******************************************************************************
 *  *
 *  *
 *  * This program and the accompanying materials are made available under the
 *  * terms of the Apache License, Version 2.0 which is available at
 *  * https://www.apache.org/licenses/LICENSE-2.0.
 *  *
 *  *  See the NOTICE file distributed with this work for additional
 *  *  information regarding copyright ownership.
 *  * Unless required by applicable law or agreed to in writing, software
 *  * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 *  * License for the specific language governing permissions and limitations
 *  * under the License.
 *  *
 *  * SPDX-License-Identifier: Apache-2.0
 *  *****************************************************************************
 */


#ifndef FLATBUFFERS_GENERATED_PROPERTIES_SD_GRAPH_H_
#define FLATBUFFERS_GENERATED_PROPERTIES_SD_GRAPH_H_
#include "flatbuffers/flatbuffers.h"
#include "array_generated.h"

namespace sd {
namespace graph {

struct FlatProperties;

struct FlatProperties FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_I = 6,
    VT_L = 8,
    VT_D = 10,
    VT_A = 12,
    VT_B = 14,
    VT_S = 16,
    VT_SHAPE = 18
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const flatbuffers::Vector<int32_t> *i() const {
    return GetPointer<const flatbuffers::Vector<int32_t> *>(VT_I);
  }
  const flatbuffers::Vector<int64_t> *l() const {
    return GetPointer<const flatbuffers::Vector<int64_t> *>(VT_L);
  }
  const flatbuffers::Vector<double> *d() const {
    return GetPointer<const flatbuffers::Vector<double> *>(VT_D);
  }
  const flatbuffers::Vector<flatbuffers::Offset<FlatArray>> *a() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<FlatArray>> *>(VT_A);
  }
  const flatbuffers::Vector<uint8_t> *b() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_B);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *s() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_S);
  }
  const flatbuffers::Vector<int32_t> *shape() const {
    return GetPointer<const flatbuffers::Vector<int32_t> *>(VT_SHAPE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyOffset(verifier, VT_I) &&
           verifier.VerifyVector(i()) &&
           VerifyOffset(verifier, VT_L) &&
           verifier.VerifyVector(l()) &&
           VerifyOffset(verifier, VT_D) &&
           verifier.VerifyVector(d()) &&
           VerifyOffset(verifier, VT_A) &&
           verifier.VerifyVector(a()) &&
           verifier.VerifyVectorOfTables(a()) &&
           VerifyOffset(verifier, VT_B) &&
           verifier.VerifyVector(b()) &&
           VerifyOffset(verifier, VT_S) &&
           verifier.VerifyVector(s()) &&
           verifier.VerifyVectorOfStrings(s()) &&
           VerifyOffset(verifier, VT_SHAPE) &&
           verifier.VerifyVector(shape()) &&
           verifier.EndTable();
  }
};

struct FlatPropertiesBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(FlatProperties::VT_NAME, name);
  }
  void add_i(flatbuffers::Offset<flatbuffers::Vector<int32_t>> i) {
    fbb_.AddOffset(FlatProperties::VT_I, i);
  }
  void add_l(flatbuffers::Offset<flatbuffers::Vector<int64_t>> l) {
    fbb_.AddOffset(FlatProperties::VT_L, l);
  }
  void add_d(flatbuffers::Offset<flatbuffers::Vector<double>> d) {
    fbb_.AddOffset(FlatProperties::VT_D, d);
  }
  void add_a(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<FlatArray>>> a) {
    fbb_.AddOffset(FlatProperties::VT_A, a);
  }
  void add_b(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> b) {
    fbb_.AddOffset(FlatProperties::VT_B, b);
  }
  void add_s(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> s) {
    fbb_.AddOffset(FlatProperties::VT_S, s);
  }
  void add_shape(flatbuffers::Offset<flatbuffers::Vector<int32_t>> shape) {
    fbb_.AddOffset(FlatProperties::VT_SHAPE, shape);
  }
  explicit FlatPropertiesBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  FlatPropertiesBuilder &operator=(const FlatPropertiesBuilder &);
  flatbuffers::Offset<FlatProperties> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<FlatProperties>(end);
    return o;
  }
};

inline flatbuffers::Offset<FlatProperties> CreateFlatProperties(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<flatbuffers::Vector<int32_t>> i = 0,
    flatbuffers::Offset<flatbuffers::Vector<int64_t>> l = 0,
    flatbuffers::Offset<flatbuffers::Vector<double>> d = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<FlatArray>>> a = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> b = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> s = 0,
    flatbuffers::Offset<flatbuffers::Vector<int32_t>> shape = 0) {
  FlatPropertiesBuilder builder_(_fbb);
  builder_.add_shape(shape);
  builder_.add_s(s);
  builder_.add_b(b);
  builder_.add_a(a);
  builder_.add_d(d);
  builder_.add_l(l);
  builder_.add_i(i);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<FlatProperties> CreateFlatPropertiesDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const std::vector<int32_t> *i = nullptr,
    const std::vector<int64_t> *l = nullptr,
    const std::vector<double> *d = nullptr,
    const std::vector<flatbuffers::Offset<FlatArray>> *a = nullptr,
    const std::vector<uint8_t> *b = nullptr,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *s = nullptr,
    const std::vector<int32_t> *shape = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto i__ = i ? _fbb.CreateVector<int32_t>(*i) : 0;
  auto l__ = l ? _fbb.CreateVector<int64_t>(*l) : 0;
  auto d__ = d ? _fbb.CreateVector<double>(*d) : 0;
  auto a__ = a ? _fbb.CreateVector<flatbuffers::Offset<FlatArray>>(*a) : 0;
  auto b__ = b ? _fbb.CreateVector<uint8_t>(*b) : 0;
  auto s__ = s ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*s) : 0;
  auto shape__ = shape ? _fbb.CreateVector<int32_t>(*shape) : 0;
  return CreateFlatProperties(
      _fbb,
      name__,
      i__,
      l__,
      d__,
      a__,
      b__,
      s__,
      shape__);
}

inline const FlatProperties *GetFlatProperties(const void *buf) {
  return flatbuffers::GetRoot<FlatProperties>(buf);
}

inline const FlatProperties *GetSizePrefixedFlatProperties(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<FlatProperties>(buf);
}

inline bool VerifyFlatPropertiesBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<FlatProperties>(nullptr);
}

inline bool VerifySizePrefixedFlatPropertiesBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<FlatProperties>(nullptr);
}

inline void FinishFlatPropertiesBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<FlatProperties> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedFlatPropertiesBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<FlatProperties> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace graph
}  // namespace sd

#endif  // FLATBUFFERS_GENERATED_PROPERTIES_SD_GRAPH_H_
