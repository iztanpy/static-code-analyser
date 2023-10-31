#include "qps/clauses/suchthat_clauses/affects.h"

Constraint Affects::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{AffectsEvaluator::Handle(lhs_arg, rhs_arg, pkb_reader)};
  }, this->lhs, this->rhs);
}

void Affects::Validate() {
  SuchThatValidator::ValidateBothStmt(lhs, rhs);
}
