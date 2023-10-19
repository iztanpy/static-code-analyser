#include "qps/clauses/select_clause.h"

bool SelectClause::equals(const SelectClause& other) const {
  return declaration == other.declaration;
}

Constraint SelectClause::Evaluate(ReadFacade& pkb_reader) {
  return SelectEvaluator::Evaluate(declaration, pkb_reader);
}

std::unordered_set<Synonym> SelectClause::GetSynonyms() {
  return {declaration.synonym};
}

size_t SelectClause::Hash() {
  uint64_t result = Clause::Hash();
  result = result * 31 + std::hash<Declaration>()(declaration);
  return static_cast<size_t>(result);
}

bool SelectClause::equals(const Clause* other) const {
  const auto* other_clause = dynamic_cast<const SelectClause*>(other);
  return other_clause != nullptr && this == other_clause;
}

bool operator==(const SelectClause& lhs, const SelectClause& rhs) {
  return lhs.GetRelRef() == rhs.GetRelRef() && lhs.declaration == rhs.declaration;
}
