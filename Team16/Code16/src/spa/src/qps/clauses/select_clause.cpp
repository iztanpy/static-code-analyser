#include "qps/clauses/select_clause.h"

bool SelectClause::equals(const SelectClause& other) const {
  return this->declaration.equals(other.declaration);
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
