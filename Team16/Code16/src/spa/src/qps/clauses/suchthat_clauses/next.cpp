#include "qps/clauses/suchthat_clauses/next.h"

Constraint Next::Evaluate(ReadFacade& pkb_reader) {
    return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
      return Constraint{NextEvaluator::Handle(lhs_arg, rhs_arg, pkb_reader, false)};
    }, this->lhs, this->rhs);
}

void Next::Validate() {
    SuchThatValidator::ValidateBothStmt(lhs, rhs);
}

Constraint NextT::Evaluate(ReadFacade& pkb_reader) {
    return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
      return Constraint{NextEvaluator::Handle(lhs_arg, rhs_arg, pkb_reader, true)};
    }, this->lhs, this->rhs);
}

void NextT::Validate() {
    SuchThatValidator::ValidateBothStmt(lhs, rhs);
}
