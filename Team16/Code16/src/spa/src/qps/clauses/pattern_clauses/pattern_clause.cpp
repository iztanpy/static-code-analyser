#include "qps/clauses/pattern_clauses/pattern_clause.h"

Constraint AssignPattern::Evaluate(ReadFacade& pkb_reader) {
//  return std::visit([this, &pkb_reader](auto&& lhs_arg) {
//    return Constraint{AssignPatternEvaluator::Handle(this->syn_assignment.synonym, lhs_arg, this->rhs, pkb_reader)};
//  }, this->lhs);
}

void AssignPattern::Validate() {
  PatternValidator::Validate(syn_assignment, lhs);
}

bool PatternClause::are_expr_spec_equal(ExprSpec expr_1, ExprSpec expr_2) {
  if (std::holds_alternative<ExactExpr>(expr_1) && std::holds_alternative<ExactExpr>(expr_2)) {
    return std::get<ExactExpr>(expr_1).value == std::get<ExactExpr>(expr_2).value;
  } else if (std::holds_alternative<PartialExpr>(expr_1) && std::holds_alternative<PartialExpr>(expr_2)) {
    return std::get<PartialExpr>(expr_1).value == std::get<PartialExpr>(expr_2).value;
  } else if (std::holds_alternative<Wildcard>(expr_1) && std::holds_alternative<Wildcard>(expr_2)) {
    return true;
  } else {
    return false;
  }
}
