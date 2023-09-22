#include "qps/clauses/suchthat_clauses/modifies.h"

Constraint ModifiesP::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{ModifiesEvaluator::Handle(lhs_arg, rhs_arg, pkb_reader)};
  }, this->lhs, this->rhs);
}

void ModifiesP::Validate() {
  SuchThatValidator::ValidateModifies(lhs, rhs);
}

Constraint ModifiesS::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{ModifiesEvaluator::Handle(lhs_arg, rhs_arg, pkb_reader)};
  }, this->lhs, this->rhs);
}

void ModifiesS::Validate() {
  SuchThatValidator::ValidateModifies(lhs, rhs);
}
