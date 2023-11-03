#include "qps/clauses/select_clause.h"

bool SelectClause::equals(const SelectClause& other) const {
  return attr_ref == other.attr_ref;
}

Constraint SelectClause::Evaluate(ReadFacade& pkb_reader) {
  return attr_ref.Evaluate(pkb_reader);
}

std::unordered_set<Declaration> SelectClause::ComputeSynonyms() const {
  std::vector<Declaration> synonyms = attr_ref.GetSynonyms();
  // Check if synonyms vector size is not 1 or 2
  if (synonyms.empty() || synonyms.size() > 2) {
    throw std::runtime_error("SelectClause::GetSelectedSynonym. Synonyms size must be 1 or 2.");
  }
  return {synonyms.begin(), synonyms.end()};
}

Declaration SelectClause::GetSelectedSynonym() const {
  std::vector<Declaration> synonyms = attr_ref.GetSynonyms();
  // Check if synonyms vector size is not 1 or 2
  if (synonyms.empty() || synonyms.size() > 2) {
    throw std::runtime_error("SelectClause::GetSelectedSynonym. Synonyms size must be 1 or 2.");
  }
  // Return the second element if there are two synonyms, otherwise return the first
  return synonyms.back();
}

size_t SelectClause::Hash() const {
  uint64_t result = Clause::Hash();
  result = result * 31 + attr_ref.Hash();
  return static_cast<size_t>(result);
}

bool SelectClause::equals(const Clause* other) const {
  if (!Clause::equals(other)) {
    return false;
  }
  const auto* other_clause = dynamic_cast<const SelectClause*>(other);
  return other_clause != nullptr && *this == *other_clause;
}

bool operator==(const SelectClause& lhs, const SelectClause& rhs) {
  return lhs.attr_ref == rhs.attr_ref;
}
