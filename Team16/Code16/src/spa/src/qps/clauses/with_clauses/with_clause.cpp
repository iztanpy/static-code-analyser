#include "qps/clauses/with_clauses/with_clause.h"

std::unordered_set<Synonym> WithClause::GetSynonyms() const {
  std::unordered_set<Synonym> synonyms;
  if (std::holds_alternative<AttrRef>(lhs)) {
    synonyms.insert(std::get<AttrRef>(lhs).GetSynonym());
  }
  if (std::holds_alternative<AttrRef>(rhs)) {
    synonyms.insert(std::get<AttrRef>(rhs).GetSynonym());
  }
  return synonyms;
}

Constraint WithClause::Evaluate(ReadFacade& pkb_reader) {
  return Constraint();
}

size_t WithClause::Hash() const {
  uint64_t result = Clause::Hash();
  result = result * 31 + std::hash<Ref>()(lhs);
  result = result * 31 + std::hash<Ref>()(rhs);
  return static_cast<size_t>(result);
}

bool WithClause::equals(const Clause* other) const {
  const auto* other_clause = dynamic_cast<const WithClause*>(other);
  return other_clause != nullptr && *this == *other_clause;
}

bool operator==(const WithClause& first, const WithClause& second) {
  return first.lhs == second.lhs && first.rhs == second.rhs;
}

void WithClause::Validate() {
  // TODO(Cuong): Implement validate
}
