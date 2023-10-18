#include "qps/query_parser/clause_builder/select_clause_builder.h"

SelectClauseBuilder::SelectClauseBuilder() = default;

void SelectClauseBuilder::setDeclaration(Declaration declaration) {
  selectClause.declaration = std::move(declaration);
}

std::unique_ptr<SelectClause> SelectClauseBuilder::getClause() const {
  return std::make_unique<SelectClause>(selectClause);
}
