#include "qps/clauses/pattern_clauses/pattern_clause.h"

std::unordered_set<Declaration> PatternClause::ComputeSynonyms() const {
  std::unordered_set < Declaration > synonyms;
  synonyms.insert(declaration);

  if (std::holds_alternative<Declaration>(lhs)) {
    synonyms.insert(std::get<Declaration>(lhs));
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
