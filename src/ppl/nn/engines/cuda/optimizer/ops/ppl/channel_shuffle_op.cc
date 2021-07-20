// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "ppl/nn/engines/cuda/optimizer/ops/ppl/channel_shuffle_op.h"

#include "ppl/nn/common/logger.h"
#include "ppl/nn/engines/cuda/kernels/ppl/channel_shuffle_kernel.h"

using namespace std;
using namespace ppl::common;
using namespace ppl::nn::common;

namespace ppl { namespace nn { namespace cuda {

RetCode ChannelShuffleOp::Init(const OptKernelOptions& options) {
    LOG(ERROR) << "init channel shuffle";
    infer_type_func_ = [this](InputOutputInfo* info, datatype_t type) -> RetCode {
        LOG(ERROR) << type;
        LOG(ERROR) << info->GetOutputCount();
        LOG(ERROR) << info->GetInputCount();
        auto in_shape = &info->GetInput<TensorImpl>(0)->GetShape();
        LOG(ERROR) << in_shape->GetDataType();
        for (uint32_t i = 0; i < info->GetOutputCount(); ++i) {
            auto out_shape = &info->GetOutput<TensorImpl>(i)->GetShape();
            out_shape->SetDataType(in_shape->GetDataType());
        }
        return ppl::common::RC_SUCCESS;
        // return type != DATATYPE_UNKNOWN ? InferDefaultType(info, type) : InferInheritedType(info);
    };

    infer_dims_func_ = [this](InputOutputInfo* info) -> RetCode {
        auto& in_shape0 = info->GetInput<TensorImpl>(0)->GetShape();
        info->GetOutput<TensorImpl>(0)->GetShape().Reshape(in_shape0.GetDims(), in_shape0.GetRealDimCount());
        return RC_SUCCESS;
    };
    
    return RC_SUCCESS;
}

RetCode ChannelShuffleOp::Finalize(const OptKernelOptions& options) {
    auto status = SetCommonParam(options);
    if (status != RC_SUCCESS) {
        LOG(ERROR) << "load common param failed: " << GetRetCodeStr(status);
        return status;
    }

    return RC_SUCCESS;
}

KernelImpl* ChannelShuffleOp::CreateKernelImpl() const {
    return CreateKernelImplWithParam<ChannelShuffleKernel>(&param_);
}

}}} // namespace ppl::nn::cuda