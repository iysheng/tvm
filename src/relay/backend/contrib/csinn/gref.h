/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*!
 * \file src/relay/backend/contrib/csinn/gref.h
 * \brief The base class for gref.
 */
#ifndef TVM_RELAY_BACKEND_CONTRIB_CSINN_GREF_H_
#define TVM_RELAY_BACKEND_CONTRIB_CSINN_GREF_H_

#include <string>
#include <vector>

#include "csinn.h"

namespace tvm {
namespace relay {
namespace contrib {

class CodegenGref : public CodegenCSINN {
 public:
  CodegenGref() : CodegenCSINN() {
    auto ctx = transform::PassContext::Current();
    auto cfg = ctx->GetConfig<CSINNConfig>("relay.ext.csinn.options");
    this->rmode_ = cfg.value()->run_mode;
    if (cfg.value()->target == "c906") {
      this->target_ = "CSINN_C906";
    } else if (cfg.value()->target == "c908") {
      this->target_ = "CSINN_C908";
    } else if (cfg.value()->target == "i805") {
      this->target_ = "CSINN_I805";
      base_dtype_ = "CSINN_DTYPE_UINT8";
    } else {
      this->target_ = "CSINN_GREF";
    }
  }
  virtual ~CodegenGref() {}
  virtual string EmitGraph(void);
  void malloc_buf(string out, int out_size) {}
  void CreateMallocBuf(string name, std::vector<int> shape, string dtype) {}
  void CreateTensorSessData() {}
  void CreateHybridTensorSessData(std::vector<int> shape, string dtype) {}
  void FreeTensor(const Expr& expr, string name) {}
  // void GetAsymScale(float min_value, float max_value, int bits, Qinfo* qinfo);

  void SessionRunMode() { PrintOneLine(code_stream_, "sess->base_run_mode = CSINN_RM_CPU_GRAPH;"); }

 private:
  string rmode_{""};
  string target_{""};
};

}  // namespace contrib
}  // namespace relay
}  // namespace tvm
#endif  // TVM_RELAY_BACKEND_CONTRIB_CSINN_GREF_H_
