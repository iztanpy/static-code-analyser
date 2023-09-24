#include "qps/query_parser/clause_builder/select_clause_builder.h"
#include <string>
#include <utility>

SelectClauseBuilder::SelectClauseBuilder() = default;

void SelectClauseBuilder::setDeclaration(Declaration declaration) {
  selectClause.declaration = std::move(declaration);
}

SelectClause SelectClauseBuilder::getClause() const {
  return selectClause;
}
