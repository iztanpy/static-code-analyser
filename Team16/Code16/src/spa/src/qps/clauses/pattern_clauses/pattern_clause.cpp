#include "qps/clauses/pattern_clauses/pattern_clause.h"

std::unordered_set<Synonym> PatternClause::GetSynonyms() {
  std::unordered_set < Synonym > synonyms;
  synonyms.insert(declaration.synonym);

  if (std::holds_alternative<Declaration>(lhs)) {
    synonyms.insert(std::get<Declaration>(lhs).synonym);
  }

  return synonyms;
}

Constraint AssignPattern::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{AssignPatternEvaluator::Handle(this->declaration.synonym, lhs_arg, rhs_arg, pkb_reader)};
  }, lhs, rhs);
}

void AssignPattern::Validate() {
  PatternValidator::Validate(declaration, lhs);
}

size_t AssignPattern::Hash() {
  uint64_t result = PatternClause::Hash();
  result = result * 31 + std::hash<ExprSpec>{}(rhs);
  return static_cast<size_t>(result);
}

Constraint WhilePattern::Evaluate(ReadFacade& pkb_reader) {
  return Constraint();
}

void WhilePattern::Validate() {
  // TODO(Cuong): validate
}

Constraint IfPattern::Evaluate(ReadFacade& pkb_reader) {
  return Constraint();
}

void IfPattern::Validate() {
  // TODO(Cuong): validate
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

size_t PatternClause::Hash() {
  uint64_t result = Clause::Hash();
  result = result * 31 + std::hash<Declaration>{}(declaration);
  result = result * 31 + std::hash<EntRef>{}(lhs);
  return static_cast<size_t>(result);
}
