#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "cinn/hlir/framework/node.h"
#include "cinn/hlir/framework/schedule.h"
#include "cinn/lang/packed_func.h"

namespace cinn {
namespace hlir {
namespace framework {

using CINNCompute  = lang::PackedFunc;
using CINNSchedule = lang::PackedFunc;

class OpStrategy;

using StrategyFunction   = std::function<std::shared_ptr<OpStrategy>(const NodeAttr&,
                                                                   const std::vector<ir::Tensor>&,
                                                                   const std::vector<Type>&,
                                                                   const std::vector<std::vector<int>>&,
                                                                   const common::Target&)>;
using InferShapeFunction = std::function<std::vector<std::vector<int>>(
    const std::vector<std::vector<int>>&, NodeAttr&, const common::Target&)>;

//! Operator implementation that includes compute and schedule function.
class OpImpl : public common::Object {
 public:
  //! Compute function
  CINNCompute fcompute;
  //! Schedule function
  CINNSchedule fschedule;
  //! Name of the implementation
  std::string name;
  //! Priority level
  int plevel;
  /**
   * \brief Invoke the operator compute function.
   * @param attrs The attribute of the primitive
   * @param inputs The input tensors.
   * @param out_type The output type information.
   * @return The output compute description of the operator.
   */
  ir::Tensor Compute(const std::vector<ir::Tensor>& inputs, const Type& out_type) {
    // TODO(haozech) : add support for packedfunc to return Tensor
    // Expected : return this->fcompute(inputs, out_type);
    ir::Tensor temp;
    return temp;
  }
  /**
   * \brief Build the computation schedule.
   * @param attrs The attribute of the node.
   * @param outs The output tensors.
   * @param target The build target.
   * @return The computation schedule.
   */
  common::Shared<Schedule> GetSchedule(const std::vector<ir::Tensor>& outs,
                                       const std::vector<ir::Tensor>& temp_tensors,
                                       const Target& target) {
    // TODO(haozech) : add support for packedfunc to return Schedule
    // Expected : return this->fschedule(outs, target);
    return nullptr;
  }

  const char* type_info() const override { return __type_info__; }

 private:
  static constexpr char* __type_info__ = "OpImplementation";
};

//! Specialized implementations for operators under certain conditions.
class OpSpec : public common::Object {
 public:
  //! List of implementations.
  std::vector<std::shared_ptr<OpImpl>> implementations;

  /** \brief Condition to enable the specialization.
   *    Could be undefined to represent generic case.
   *  TODO(haozech) : build a specified class SpecializedCondition to represent the condition.
   *  Expected : SpecializedCondition condition;
   */
  std::string condition;

  const char* type_info() const override { return __type_info__; }

  void AddImpl(CINNCompute fcompute, CINNSchedule fschedule, std::string name, int plevel) {
    auto n       = std::make_shared<OpImpl>();
    n->fcompute  = fcompute;
    n->fschedule = fschedule;
    n->name      = std::move(name);
    n->plevel    = plevel;
    this->implementations.push_back(n);
  }

 private:
  static constexpr char* __type_info__ = "OpSpecialization";
};

//! Operator strategy class.
class OpStrategy : public common::Object {
 public:
  const char* type_info() const override { return __type_info__; }
  //! List of operator specializations.
  std::vector<std::shared_ptr<OpSpec>> specializations;

  /**
   * \brief Add an implementation.
   * @param fcompute Compute function
   * @param fschedule Schedule function
   * @param name Name of the implementation
   * @param plevel Priority level of the implementation
   */
  void AddImpl(CINNCompute fcompute, CINNSchedule fschedule, std::string name, int plevel);
  static std::shared_ptr<OpImpl> SelectImpl(const std::shared_ptr<OpStrategy>& strategy);

 private:
  static constexpr char* __type_info__ = "OpStrategy";
};

}  // namespace framework
}  // namespace hlir
}  // namespace cinn
