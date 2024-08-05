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


#ifndef FLATBUFFERS_GENERATED_UIGRAPHSTATIC_SD_GRAPH_H_
#define FLATBUFFERS_GENERATED_UIGRAPHSTATIC_SD_GRAPH_H_

#include "flatbuffers/flatbuffers.h"

#include "array_generated.h"
#include "utils_generated.h"
#include "variable_generated.h"

namespace sd {
namespace graph {

struct UIStaticInfoRecord;
struct UIStaticInfoRecordBuilder;

struct UISystemInfo;
struct UISystemInfoBuilder;

struct UIGraphStructure;
struct UIGraphStructureBuilder;

struct UIVariable;
struct UIVariableBuilder;

struct UIOp;
struct UIOpBuilder;

enum UIInfoType {
  UIInfoType_GRAPH_STRUCTURE = 0,
  UIInfoType_SYTEM_INFO = 1,
  UIInfoType_START_EVENTS = 2,
  UIInfoType_MIN = UIInfoType_GRAPH_STRUCTURE,
  UIInfoType_MAX = UIInfoType_START_EVENTS
};

inline const UIInfoType (&EnumValuesUIInfoType())[3] {
  static const UIInfoType values[] = {
    UIInfoType_GRAPH_STRUCTURE,
    UIInfoType_SYTEM_INFO,
    UIInfoType_START_EVENTS
  };
  return values;
}

inline const char * const *EnumNamesUIInfoType() {
  static const char * const names[4] = {
    "GRAPH_STRUCTURE",
    "SYTEM_INFO",
    "START_EVENTS",
    nullptr
  };
  return names;
}

inline const char *EnumNameUIInfoType(UIInfoType e) {
  if (flatbuffers::IsOutRange(e, UIInfoType_GRAPH_STRUCTURE, UIInfoType_START_EVENTS)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesUIInfoType()[index];
}

struct UIStaticInfoRecord FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef UIStaticInfoRecordBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE { VT_INFOTYPE = 4 };
  UIInfoType infoType() const {
    return static_cast<UIInfoType>(GetField<int8_t>(VT_INFOTYPE, 0));
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_INFOTYPE) &&
           verifier.EndTable();
  }
};

struct UIStaticInfoRecordBuilder {
  typedef UIStaticInfoRecord Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_infoType(UIInfoType infoType) {
    fbb_.AddElement<int8_t>(UIStaticInfoRecord::VT_INFOTYPE, static_cast<int8_t>(infoType), 0);
  }
  explicit UIStaticInfoRecordBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  UIStaticInfoRecordBuilder &operator=(const UIStaticInfoRecordBuilder &);
  flatbuffers::Offset<UIStaticInfoRecord> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<UIStaticInfoRecord>(end);
    return o;
  }
};

inline flatbuffers::Offset<UIStaticInfoRecord> CreateUIStaticInfoRecord(
    flatbuffers::FlatBufferBuilder &_fbb, UIInfoType infoType = UIInfoType_GRAPH_STRUCTURE) {
  UIStaticInfoRecordBuilder builder_(_fbb);
  builder_.add_infoType(infoType);
  return builder_.Finish();
}

struct UISystemInfo FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef UISystemInfoBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PHYSICALCORES = 4
  };
  int32_t physicalCores() const {
    return GetField<int32_t>(VT_PHYSICALCORES, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_PHYSICALCORES) &&
           verifier.EndTable();
  }
};

struct UISystemInfoBuilder {
  typedef UISystemInfo Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_physicalCores(int32_t physicalCores) {
    fbb_.AddElement<int32_t>(UISystemInfo::VT_PHYSICALCORES, physicalCores, 0);
  }
  explicit UISystemInfoBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  UISystemInfoBuilder &operator=(const UISystemInfoBuilder &);
  flatbuffers::Offset<UISystemInfo> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<UISystemInfo>(end);
    return o;
  }
};

inline flatbuffers::Offset<UISystemInfo> CreateUISystemInfo(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t physicalCores = 0) {
  UISystemInfoBuilder builder_(_fbb);
  builder_.add_physicalCores(physicalCores);
  return builder_.Finish();
}

struct UIGraphStructure FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef UIGraphStructureBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_INPUTS = 4,
    VT_INPUTSPAIR = 6,
    VT_OUTPUTS = 8,
    VT_VARIABLES = 10,
    VT_OPS = 12
  };
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *inputs() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_INPUTS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<IntPair>> *inputsPair() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<IntPair>> *>(VT_INPUTSPAIR);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *outputs() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_OUTPUTS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<UIVariable>> *variables() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<UIVariable>> *>(VT_VARIABLES);
  }
  const flatbuffers::Vector<flatbuffers::Offset<UIOp>> *ops() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<UIOp>> *>(VT_OPS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_INPUTS) &&
           verifier.VerifyVector(inputs()) &&
           verifier.VerifyVectorOfStrings(inputs()) &&
           VerifyOffset(verifier, VT_INPUTSPAIR) &&
           verifier.VerifyVector(inputsPair()) &&
           verifier.VerifyVectorOfTables(inputsPair()) &&
           VerifyOffset(verifier, VT_OUTPUTS) &&
           verifier.VerifyVector(outputs()) &&
           verifier.VerifyVectorOfStrings(outputs()) &&
           VerifyOffset(verifier, VT_VARIABLES) &&
           verifier.VerifyVector(variables()) &&
           verifier.VerifyVectorOfTables(variables()) &&
           VerifyOffset(verifier, VT_OPS) &&
           verifier.VerifyVector(ops()) &&
           verifier.VerifyVectorOfTables(ops()) &&
           verifier.EndTable();
  }
};

struct UIGraphStructureBuilder {
  typedef UIGraphStructure Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_inputs(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> inputs) {
    fbb_.AddOffset(UIGraphStructure::VT_INPUTS, inputs);
  }
  void add_inputsPair(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<IntPair>>> inputsPair) {
    fbb_.AddOffset(UIGraphStructure::VT_INPUTSPAIR, inputsPair);
  }
  void add_outputs(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> outputs) {
    fbb_.AddOffset(UIGraphStructure::VT_OUTPUTS, outputs);
  }
  void add_variables(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<UIVariable>>> variables) {
    fbb_.AddOffset(UIGraphStructure::VT_VARIABLES, variables);
  }
  void add_ops(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<UIOp>>> ops) {
    fbb_.AddOffset(UIGraphStructure::VT_OPS, ops);
  }
  explicit UIGraphStructureBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  UIGraphStructureBuilder &operator=(const UIGraphStructureBuilder &);
  flatbuffers::Offset<UIGraphStructure> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<UIGraphStructure>(end);
    return o;
  }
};

inline flatbuffers::Offset<UIGraphStructure> CreateUIGraphStructure(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> inputs = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<IntPair>>> inputsPair = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> outputs = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<UIVariable>>> variables = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<UIOp>>> ops = 0) {
  UIGraphStructureBuilder builder_(_fbb);
  builder_.add_ops(ops);
  builder_.add_variables(variables);
  builder_.add_outputs(outputs);
  builder_.add_inputsPair(inputsPair);
  builder_.add_inputs(inputs);
  return builder_.Finish();
}

inline flatbuffers::Offset<UIGraphStructure> CreateUIGraphStructureDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *inputs = nullptr,
    const std::vector<flatbuffers::Offset<IntPair>> *inputsPair = nullptr,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *outputs = nullptr,
    const std::vector<flatbuffers::Offset<UIVariable>> *variables = nullptr,
    const std::vector<flatbuffers::Offset<UIOp>> *ops = nullptr) {
  auto inputs__ = inputs ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*inputs) : 0;
  auto inputsPair__ = inputsPair ? _fbb.CreateVector<flatbuffers::Offset<IntPair>>(*inputsPair) : 0;
  auto outputs__ = outputs ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*outputs) : 0;
  auto variables__ = variables ? _fbb.CreateVector<flatbuffers::Offset<UIVariable>>(*variables) : 0;
  auto ops__ = ops ? _fbb.CreateVector<flatbuffers::Offset<UIOp>>(*ops) : 0;
  return CreateUIGraphStructure(
      _fbb,
      inputs__,
      inputsPair__,
      outputs__,
      variables__,
      ops__);
}

struct UIVariable FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef UIVariableBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_NAME = 6,
    VT_TYPE = 8,
    VT_DATATYPE = 10,
    VT_SHAPE = 12,
    VT_CONTROLDEPS = 14,
    VT_OUTPUTOFOP = 16,
    VT_INPUTSFOROP = 18,
    VT_CONTROLDEPSFOROP = 20,
    VT_CONTROLDEPSFORVAR = 22,
    VT_GRADIENTVARIABLE = 24,
    VT_UILABELEXTRA = 26,
    VT_CONSTANTVALUE = 28
  };
  const IntPair *id() const {
    return GetPointer<const IntPair *>(VT_ID);
  }
  const flatbuffers::String *name() const { return GetPointer<const flatbuffers::String *>(VT_NAME); }
  VarType type() const { return static_cast<VarType>(GetField<int8_t>(VT_TYPE, 0)); }
  DType datatype() const {
    return static_cast<DType>(GetField<int8_t>(VT_DATATYPE, 0));
  }
  const flatbuffers::Vector<int64_t> *shape() const {
    return GetPointer<const flatbuffers::Vector<int64_t> *>(VT_SHAPE);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *controlDeps() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_CONTROLDEPS);
  }
  const flatbuffers::String *outputOfOp() const {
    return GetPointer<const flatbuffers::String *>(VT_OUTPUTOFOP);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *inputsForOp() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_INPUTSFOROP);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *controlDepsForOp() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_CONTROLDEPSFOROP);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *controlDepsForVar() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_CONTROLDEPSFORVAR);
  }
  const flatbuffers::String *gradientVariable() const {
    return GetPointer<const flatbuffers::String *>(VT_GRADIENTVARIABLE);
  }
  const flatbuffers::String *uiLabelExtra() const {
    return GetPointer<const flatbuffers::String *>(VT_UILABELEXTRA);
  }
  const FlatArray *constantValue() const {
    return GetPointer<const FlatArray *>(VT_CONSTANTVALUE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ID) &&
           verifier.VerifyTable(id()) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<int8_t>(verifier, VT_TYPE) &&
           VerifyField<int8_t>(verifier, VT_DATATYPE) &&
           VerifyOffset(verifier, VT_SHAPE) &&
           verifier.VerifyVector(shape()) &&
           VerifyOffset(verifier, VT_CONTROLDEPS) &&
           verifier.VerifyVector(controlDeps()) &&
           verifier.VerifyVectorOfStrings(controlDeps()) &&
           VerifyOffset(verifier, VT_OUTPUTOFOP) &&
           verifier.VerifyString(outputOfOp()) &&
           VerifyOffset(verifier, VT_INPUTSFOROP) &&
           verifier.VerifyVector(inputsForOp()) &&
           verifier.VerifyVectorOfStrings(inputsForOp()) &&
           VerifyOffset(verifier, VT_CONTROLDEPSFOROP) &&
           verifier.VerifyVector(controlDepsForOp()) &&
           verifier.VerifyVectorOfStrings(controlDepsForOp()) &&
           VerifyOffset(verifier, VT_CONTROLDEPSFORVAR) &&
           verifier.VerifyVector(controlDepsForVar()) &&
           verifier.VerifyVectorOfStrings(controlDepsForVar()) &&
           VerifyOffset(verifier, VT_GRADIENTVARIABLE) &&
           verifier.VerifyString(gradientVariable()) &&
           VerifyOffset(verifier, VT_UILABELEXTRA) &&
           verifier.VerifyString(uiLabelExtra()) &&
           VerifyOffset(verifier, VT_CONSTANTVALUE) &&
           verifier.VerifyTable(constantValue()) &&
           verifier.EndTable();
  }
};

struct UIVariableBuilder {
  typedef UIVariable Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(flatbuffers::Offset<IntPair> id) {
    fbb_.AddOffset(UIVariable::VT_ID, id);
  }
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(UIVariable::VT_NAME, name);
  }
  void add_type(VarType type) {
    fbb_.AddElement<int8_t>(UIVariable::VT_TYPE, static_cast<int8_t>(type), 0);
  }
  void add_datatype(DType datatype) {
    fbb_.AddElement<int8_t>(UIVariable::VT_DATATYPE, static_cast<int8_t>(datatype), 0);
  }
  void add_shape(flatbuffers::Offset<flatbuffers::Vector<int64_t>> shape) {
    fbb_.AddOffset(UIVariable::VT_SHAPE, shape);
  }
  void add_controlDeps(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> controlDeps) {
    fbb_.AddOffset(UIVariable::VT_CONTROLDEPS, controlDeps);
  }
  void add_outputOfOp(flatbuffers::Offset<flatbuffers::String> outputOfOp) {
    fbb_.AddOffset(UIVariable::VT_OUTPUTOFOP, outputOfOp);
  }
  void add_inputsForOp(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> inputsForOp) {
    fbb_.AddOffset(UIVariable::VT_INPUTSFOROP, inputsForOp);
  }
  void add_controlDepsForOp(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> controlDepsForOp) {
    fbb_.AddOffset(UIVariable::VT_CONTROLDEPSFOROP, controlDepsForOp);
  }
  void add_controlDepsForVar(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> controlDepsForVar) {
    fbb_.AddOffset(UIVariable::VT_CONTROLDEPSFORVAR, controlDepsForVar);
  }
  void add_gradientVariable(flatbuffers::Offset<flatbuffers::String> gradientVariable) {
    fbb_.AddOffset(UIVariable::VT_GRADIENTVARIABLE, gradientVariable);
  }
  void add_uiLabelExtra(flatbuffers::Offset<flatbuffers::String> uiLabelExtra) {
    fbb_.AddOffset(UIVariable::VT_UILABELEXTRA, uiLabelExtra);
  }
  void add_constantValue(flatbuffers::Offset<FlatArray> constantValue) {
    fbb_.AddOffset(UIVariable::VT_CONSTANTVALUE, constantValue);
  }
  explicit UIVariableBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  UIVariableBuilder &operator=(const UIVariableBuilder &);
  flatbuffers::Offset<UIVariable> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<UIVariable>(end);
    return o;
  }
};

inline flatbuffers::Offset<UIVariable> CreateUIVariable(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<IntPair> id = 0,
    flatbuffers::Offset<flatbuffers::String> name = 0, VarType type = VarType_VARIABLE, DType datatype = DType_INHERIT,
    flatbuffers::Offset<flatbuffers::Vector<int64_t>> shape = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> controlDeps = 0,
    flatbuffers::Offset<flatbuffers::String> outputOfOp = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> inputsForOp = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> controlDepsForOp = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> controlDepsForVar = 0,
    flatbuffers::Offset<flatbuffers::String> gradientVariable = 0,
    flatbuffers::Offset<flatbuffers::String> uiLabelExtra = 0,
    flatbuffers::Offset<FlatArray> constantValue = 0) {
  UIVariableBuilder builder_(_fbb);
  builder_.add_constantValue(constantValue);
  builder_.add_uiLabelExtra(uiLabelExtra);
  builder_.add_gradientVariable(gradientVariable);
  builder_.add_controlDepsForVar(controlDepsForVar);
  builder_.add_controlDepsForOp(controlDepsForOp);
  builder_.add_inputsForOp(inputsForOp);
  builder_.add_outputOfOp(outputOfOp);
  builder_.add_controlDeps(controlDeps);
  builder_.add_shape(shape);
  builder_.add_name(name);
  builder_.add_id(id);
  builder_.add_datatype(datatype);
  builder_.add_type(type);
  return builder_.Finish();
}

inline flatbuffers::Offset<UIVariable> CreateUIVariableDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<IntPair> id = 0,
    const char *name = nullptr,
    VarType type = VarType_VARIABLE, DType datatype = DType_INHERIT,
    const std::vector<int64_t> *shape = nullptr,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *controlDeps = nullptr,
    const char *outputOfOp = nullptr,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *inputsForOp = nullptr,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *controlDepsForOp = nullptr,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *controlDepsForVar = nullptr,
    const char *gradientVariable = nullptr,
    const char *uiLabelExtra = nullptr,
    flatbuffers::Offset<FlatArray> constantValue = 0) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto shape__ = shape ? _fbb.CreateVector<int64_t>(*shape) : 0;
  auto controlDeps__ = controlDeps ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*controlDeps) : 0;
  auto outputOfOp__ = outputOfOp ? _fbb.CreateString(outputOfOp) : 0;
  auto inputsForOp__ = inputsForOp ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*inputsForOp) : 0;
  auto controlDepsForOp__ = controlDepsForOp ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*controlDepsForOp) : 0;
  auto controlDepsForVar__ = controlDepsForVar ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*controlDepsForVar) : 0;
  auto gradientVariable__ = gradientVariable ? _fbb.CreateString(gradientVariable) : 0;
  auto uiLabelExtra__ = uiLabelExtra ? _fbb.CreateString(uiLabelExtra) : 0;
  return CreateUIVariable(
      _fbb,
      id,
      name__,
      type,
      datatype,
      shape__,
      controlDeps__,
      outputOfOp__,
      inputsForOp__,
      controlDepsForOp__,
      controlDepsForVar__,
      gradientVariable__,
      uiLabelExtra__,
      constantValue);
}

struct UIOp FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef UIOpBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_OPNAME = 6,
    VT_INPUTS = 8,
    VT_OUTPUTS = 10,
    VT_CONTROLDEPS = 12,
    VT_UILABELEXTRA = 14
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const flatbuffers::String *opName() const {
    return GetPointer<const flatbuffers::String *>(VT_OPNAME);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *inputs() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_INPUTS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *outputs() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_OUTPUTS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *controlDeps() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_CONTROLDEPS);
  }
  const flatbuffers::String *uiLabelExtra() const {
    return GetPointer<const flatbuffers::String *>(VT_UILABELEXTRA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyOffset(verifier, VT_OPNAME) &&
           verifier.VerifyString(opName()) &&
           VerifyOffset(verifier, VT_INPUTS) &&
           verifier.VerifyVector(inputs()) &&
           verifier.VerifyVectorOfStrings(inputs()) &&
           VerifyOffset(verifier, VT_OUTPUTS) &&
           verifier.VerifyVector(outputs()) &&
           verifier.VerifyVectorOfStrings(outputs()) &&
           VerifyOffset(verifier, VT_CONTROLDEPS) &&
           verifier.VerifyVector(controlDeps()) &&
           verifier.VerifyVectorOfStrings(controlDeps()) &&
           VerifyOffset(verifier, VT_UILABELEXTRA) &&
           verifier.VerifyString(uiLabelExtra()) &&
           verifier.EndTable();
  }
};

struct UIOpBuilder {
  typedef UIOp Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(UIOp::VT_NAME, name);
  }
  void add_opName(flatbuffers::Offset<flatbuffers::String> opName) {
    fbb_.AddOffset(UIOp::VT_OPNAME, opName);
  }
  void add_inputs(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> inputs) {
    fbb_.AddOffset(UIOp::VT_INPUTS, inputs);
  }
  void add_outputs(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> outputs) {
    fbb_.AddOffset(UIOp::VT_OUTPUTS, outputs);
  }
  void add_controlDeps(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> controlDeps) {
    fbb_.AddOffset(UIOp::VT_CONTROLDEPS, controlDeps);
  }
  void add_uiLabelExtra(flatbuffers::Offset<flatbuffers::String> uiLabelExtra) {
    fbb_.AddOffset(UIOp::VT_UILABELEXTRA, uiLabelExtra);
  }
  explicit UIOpBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  UIOpBuilder &operator=(const UIOpBuilder &);
  flatbuffers::Offset<UIOp> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<UIOp>(end);
    return o;
  }
};

inline flatbuffers::Offset<UIOp> CreateUIOp(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<flatbuffers::String> opName = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> inputs = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> outputs = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> controlDeps = 0,
    flatbuffers::Offset<flatbuffers::String> uiLabelExtra = 0) {
  UIOpBuilder builder_(_fbb);
  builder_.add_uiLabelExtra(uiLabelExtra);
  builder_.add_controlDeps(controlDeps);
  builder_.add_outputs(outputs);
  builder_.add_inputs(inputs);
  builder_.add_opName(opName);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<UIOp> CreateUIOpDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const char *opName = nullptr,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *inputs = nullptr,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *outputs = nullptr,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *controlDeps = nullptr,
    const char *uiLabelExtra = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto opName__ = opName ? _fbb.CreateString(opName) : 0;
  auto inputs__ = inputs ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*inputs) : 0;
  auto outputs__ = outputs ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*outputs) : 0;
  auto controlDeps__ = controlDeps ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*controlDeps) : 0;
  auto uiLabelExtra__ = uiLabelExtra ? _fbb.CreateString(uiLabelExtra) : 0;
  return CreateUIOp(
      _fbb,
      name__,
      opName__,
      inputs__,
      outputs__,
      controlDeps__,
      uiLabelExtra__);
}

}  // namespace graph
}  // namespace sd

#endif  // FLATBUFFERS_GENERATED_UIGRAPHSTATIC_SD_GRAPH_H_
