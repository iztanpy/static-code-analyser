#include "qps/clauses/select_clause.h"

bool SelectClause::equals(const SelectClause& other) const {
  return attr_ref == other.attr_ref;
}

Constraint SelectClause::Evaluate(ReadFacade& pkb_reader) {
  return attr_ref.Evaluate(pkb_reader);
}

std::unordered_set<Synonym> SelectClause::GetSynonyms() const {
  std::vector<Synonym> synonyms = attr_ref.GetSynonyms();
  if (synonyms.empty()) {
    throw std::runtime_error("SelectClause::GetSynonyms. Shouldn't reach here! Synonyms cannot be empty");
  }
  if (attr_ref.IsComplexCase()) {
    // For complex case, get the second synonym
    return {synonyms[1]};
  }
  return {synonyms[0]};
}

size_t SelectClause::Hash() const {
  uint64_t result = Clause::Hash();
  result = result * 31 + attr_ref.Hash();
  return static_cast<size_t>(result);
}

bool SelectClause::equals(const Clause* other) const {
  const auto* other_clause = dynamic_cast<const SelectClause*>(other);
  return other_clause != nullptr && *this == *other_clause;
}

bool operator==(const SelectClause& lhs, const SelectClause& rhs) {
  return lhs.attr_ref == rhs.attr_ref;
}
