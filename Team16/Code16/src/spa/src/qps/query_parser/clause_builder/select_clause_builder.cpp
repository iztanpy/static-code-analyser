#include "qps/query_parser/clause_builder/select_clause_builder.h"
#include <string>
#include <utility>

SelectClauseBuilder::SelectClauseBuilder() = default;

void SelectClauseBuilder::setEntity(DesignEntity entity) {
  selectClause.design_entity = entity;
}

void SelectClauseBuilder::setSynonym(std::string synonym) {
  selectClause.synonym = std::move(synonym);
}
SelectClause SelectClauseBuilder::getClause() const {
  return selectClause;
}
