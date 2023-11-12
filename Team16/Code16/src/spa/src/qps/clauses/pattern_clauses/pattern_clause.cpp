#include "qps/clauses/pattern_clauses/pattern_clause.h"

std::unordered_set<Declaration> PatternClause::ComputeSynonyms() const {
  std::unordered_set < Declaration > synonyms;
  synonyms.insert(declaration);

  if (std::holds_alternative<Declaration>(lhs)) {
    synonyms.insert(std::get<Declaration>(lhs));
  }

  return synonyms;
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

size_t PatternClause::Hash() const {
  uint64_t result = Clause::Hash();
  result = result * 31 + std::hash<Declaration>{}(declaration);
  result = result * 31 + std::hash<EntRef>{}(lhs);
  return static_cast<size_t>(result);
}
