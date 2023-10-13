#include "qps/clauses/suchthat_clauses/calls.h"

Constraint Calls::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{CallsEvaluator::Handle(lhs_arg, rhs_arg, pkb_reader, false)};
  }, this->lhs, this->rhs);
}

void Calls::Validate() {
  SuchThatValidator::ValidateCalls(lhs, rhs);
}

Constraint CallsT::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{CallsEvaluator::Handle(lhs_arg, rhs_arg, pkb_reader, true)};
  }, this->lhs, this->rhs);
}

void CallsT::Validate() {
  SuchThatValidator::ValidateCalls(lhs, rhs);
}
