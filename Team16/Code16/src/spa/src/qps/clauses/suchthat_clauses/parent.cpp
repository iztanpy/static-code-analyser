#include "qps/clauses/suchthat_clauses/parent.h"

Constraint Parent::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{ParentEvaluator::Handle(lhs_arg, rhs_arg, pkb_reader, false)};
  }, this->lhs, this->rhs);
}

void Parent::Validate() {
  SuchThatValidator::ValidateBothStmt(lhs, rhs);
}

Constraint ParentT::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{ParentEvaluator::Handle(lhs_arg, rhs_arg, pkb_reader, true)};
  }, this->lhs, this->rhs);
}

void ParentT::Validate() {
  SuchThatValidator::ValidateBothStmt(lhs, rhs);
}
