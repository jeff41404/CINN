#include "math_registors.h"

#include "cinn/backends/extern_func_jit_register.h"
#include "cinn/backends/function_prototype.h"
#include "cinn/runtime/cpu/host_intrinsics.h"
#include "mkl_math.h"

namespace cinn {
namespace hlir {
namespace instruction {
namespace x86 {
using backends::FunctionProto;

bool RegisterMklMath() {
  auto host_target = common::DefaultHostTarget();

  REGISTER_EXTERN_FUNC(cinn_mkl_tanh_v_fp32, host_target)
      .SetRetType<void>()
      .AddInputType<cinn_buffer_t *>()
      .AddOutputType<cinn_buffer_t *>()
      .SetShapeInference(FunctionProto::ShapeFollowNthArgument(0))
      .End();

  REGISTER_EXTERN_FUNC(cinn_mkl_tanh_v_fp64, host_target)
      .SetRetType<void>()
      .AddInputType<cinn_buffer_t *>()
      .AddOutputType<cinn_buffer_t *>()
      .SetShapeInference(FunctionProto::ShapeFollowNthArgument(0))
      .End();

  REGISTER_EXTERN_FUNC(cinn_mkl_exp_v_fp32, host_target)
      .SetRetType<void>()
      .AddInputType<cinn_buffer_t *>()
      .AddOutputType<cinn_buffer_t *>()
      .SetShapeInference(FunctionProto::ShapeFollowNthArgument(0))
      .End();

  return true;
}

}  // namespace x86
}  // namespace instruction
}  // namespace hlir
}  // namespace cinn