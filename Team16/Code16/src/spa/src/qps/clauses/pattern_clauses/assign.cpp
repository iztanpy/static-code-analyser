#include "qps/clauses/pattern_clauses/assign.h"

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
