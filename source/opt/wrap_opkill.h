// Copyright (c) 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SOURCE_OPT_WRAP_OPKILL_H_
#define SOURCE_OPT_WRAP_OPKILL_H_

#include "source/opt/pass.h"

namespace spvtools {
namespace opt {

// Documented in optimizer.hpp
class WrapOpKill : public Pass {
 public:
  WrapOpKill() : void_type_id_(0) {}

  const char* name() const override { return "wrap-opkill"; }

  Status Process() override;

  IRContext::Analysis GetPreservedAnalyses() override {
    return IRContext::kAnalysisDefUse |
           IRContext::kAnalysisInstrToBlockMapping |
           IRContext::kAnalysisDecorations | IRContext::kAnalysisCombinators |
           IRContext::kAnalysisNameMap | IRContext::kAnalysisBuiltinVarId |
           IRContext::kAnalysisIdToFuncMapping | IRContext::kAnalysisConstants |
           IRContext::kAnalysisTypes;
  }

 private:
  // Replaces the OpKill instruction |inst| with a function call to a function
  // that contains a single instruction, which is OpKill.  An OpUnreachable
  // instruction will be placed after the function call.
  void ReplaceWithFunctionCall(Instruction* inst);

  // Returns the id of the void type.
  uint32_t GetVoidTypeId();

  // Returns the id of the function type for a void function with no parameters.
  uint32_t GetVoidFunctionTypeId();

  // Return the id of a function that has return type void, no no parameters,
  // and contains a single instruction, which is an OpKill.
  uint32_t GetOpKillFuncId();

  // The id of the void type.  If its value is 0, then the void type has not
  // been found or created yet.
  uint32_t void_type_id_;

  // The function that is a single instruction, which is an OpKill.  The
  // function has a void return type and takes no parameters. If the function is
  // |nullptr|, then the function has not been generated.
  std::unique_ptr<Function> opkill_function_;
};

}  // namespace opt
}  // namespace spvtools

#endif  // SOURCE_OPT_WRAP_OPKILL_H_
