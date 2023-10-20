#include "qps/clauses/suchthat_clauses/follows.h"

Constraint Follows::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{FollowsEvaluator::Handle(lhs_arg, rhs_arg, pkb_reader, false)};
  }, this->lhs, this->rhs);
}
void Follows::Validate() {
  SuchThatValidator::ValidateBothStmt(lhs, rhs);
}

Constraint FollowsT::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{FollowsEvaluator::Handle(lhs_arg, rhs_arg, pkb_reader, true)};
  }, this->lhs, this->rhs);
}

void FollowsT::Validate() {
  SuchThatValidator::ValidateBothStmt(lhs, rhs);
}
