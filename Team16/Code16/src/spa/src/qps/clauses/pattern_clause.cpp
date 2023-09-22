#include <string>
#include <utility>
#include "qps/clauses/pattern_clause.h"

Constraint WildCardPattern::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("WildCardPattern::Evaluate not implemented");
}

void WildCardPattern::Validate() {
  PatternValidator::Validate(syn_assignment, lhs);
}

Constraint ExactPattern::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("ExactPattern::Evaluate not implemented");
}

void ExactPattern::Validate() {
  PatternValidator::Validate(syn_assignment, lhs);
}

Constraint PartialPattern::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("PartialPattern::Evaluate not implemented");
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
