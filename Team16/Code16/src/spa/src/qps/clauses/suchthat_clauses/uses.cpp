#include "qps/clauses/suchthat_clauses/uses.h"

Constraint UsesS::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{UsesEvaluator::Handle(lhs_arg, rhs_arg, pkb_reader)};
  }, this->lhs, this->rhs);
}

void UsesS::Validate() {
  SuchThatValidator::ValidateUses(lhs, rhs);
}

Constraint UsesP::Evaluate(ReadFacade& pkb_reader) {
//  throw QpsSemanticError("Not implemented");
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{UsesEvaluator::Handle(lhs_arg, rhs_arg, pkb_reader)};
  }, this->lhs, this->rhs);
}

void UsesP::Validate() {
  SuchThatValidator::ValidateUses(lhs, rhs);
}
