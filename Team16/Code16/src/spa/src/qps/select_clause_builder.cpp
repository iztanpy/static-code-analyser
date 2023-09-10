#include "qps/select_clause_builder.h"
#include <string>
#include <utility>

void SelectClauseBuilder::setEntity(DesignEntity entity) {
    selectClause.design_entity = entity;
}

void SelectClauseBuilder::setSynonym(std::string synonym) {
    selectClause.synonym = std::move(synonym);
}
SelectClause& SelectClauseBuilder::getClause() {
    return selectClause;
}
