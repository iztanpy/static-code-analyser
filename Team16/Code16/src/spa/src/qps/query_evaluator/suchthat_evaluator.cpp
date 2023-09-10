#include "qps/query_evaluator/suchthat_evaluator.h"

SuchThatEvaluator::SuchThatEvaluator(ReadFacade& pkb_reader,
                                     ParamRef& lhs,
                                     ParamRef& rhs)
    : pkb_reader(pkb_reader), lhs(lhs), rhs(rhs) {}

Constraint SuchThatEvaluator::Evaluate() {
  return std::visit([this](auto&& lhs_arg, auto&& rhs_arg) {
    return handle(lhs_arg, rhs_arg);
  }, lhs, rhs);
}
