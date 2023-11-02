#include "qps/clauses/pattern_clauses/pattern_clause.h"

std::unordered_set<Synonym> PatternClause::ComputeSynonyms() const {
  std::unordered_set<Synonym> synonyms;
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
  PatternValidator::ValidateAssign(declaration, lhs);
}

size_t AssignPattern::Hash() const {
  uint64_t result = PatternClause::Hash();
  result = result * 31 + std::hash<ExprSpec>{}(rhs);
  return static_cast<size_t>(result);
}

bool operator==(const PatternClause& lhs, const PatternClause& rhs) {
  return lhs.declaration == rhs.declaration && lhs.lhs == rhs.lhs;
}

bool PatternClause::equals(const Clause* other) const {
  if (!Clause::equals(other)) {
    return false;
  }
  const auto* other_clause = dynamic_cast<const PatternClause*>(other);
  return other_clause != nullptr && *this == *other_clause;
}

Constraint WhilePattern::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg) {
    return Constraint{WhileEvaluator::Handle(this->declaration.synonym, lhs_arg, pkb_reader)};
  }, lhs);
}

void WhilePattern::Validate() {
  PatternValidator::ValidateWhile(declaration, lhs);
}

Constraint IfPattern::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg) {
    return Constraint{IfEvaluator::Handle(this->declaration.synonym, lhs_arg, pkb_reader)};
  }, lhs);
}

void IfPattern::Validate() {
  PatternValidator::ValidateIf(declaration, lhs);
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

size_t PatternClause::Hash() const {
  uint64_t result = Clause::Hash();
  result = result * 31 + std::hash<Declaration>{}(declaration);
  result = result * 31 + std::hash<EntRef>{}(lhs);
  return static_cast<size_t>(result);
}

bool operator==(const AssignPattern& lhs, const AssignPattern& rhs) {
  return lhs.rhs == rhs.rhs;
}

bool AssignPattern::equals(const Clause* other) const {
  if (!PatternClause::equals(other)) {
    return false;
  }
  const auto* other_clause = dynamic_cast<const AssignPattern*>(other);
  return other_clause != nullptr && *this == *other_clause;
}
