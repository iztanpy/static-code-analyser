#include "qps/clauses/pattern_clause.h"

Constraint WildCardPattern::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg) {
    return Constraint{AssignPatternEvaluator::Handle(this->syn_assignment.synonym, lhs_arg, this->rhs, pkb_reader)};
  }, this->lhs);
}

void WildCardPattern::Validate() {
  PatternValidator::Validate(syn_assignment, lhs);
}

Constraint ExactPattern::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("[AssignPattern] ExactPattern not required in Milestone1");
}

void ExactPattern::Validate() {
  PatternValidator::Validate(syn_assignment, lhs);
}

Constraint PartialPattern::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg) {
    return Constraint{AssignPatternEvaluator::Handle(this->syn_assignment.synonym, lhs_arg, this->rhs, pkb_reader)};
  }, this->lhs);
}

void PartialPattern::Validate() {
  PatternValidator::Validate(syn_assignment, lhs);
}

bool PatternClause::are_expr_spec_equal(ExprSpec expr_1, ExprSpec expr_2) {
  if (std::holds_alternative<std::string>(expr_1) && std::holds_alternative<std::string>(expr_2)) {
    return std::get<std::string>(expr_1) == std::get<std::string>(expr_2);
  } else if (std::holds_alternative<Wildcard>(expr_1) && std::holds_alternative<Wildcard>(expr_2)) {
    return true;
  } else {
    return false;
  }
}
