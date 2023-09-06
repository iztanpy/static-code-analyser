#include "SelectClauseBuilder.h"
#include <string>

void SelectClauseBuilder::setDeclaration(std::string declaration) {
    selectClause.declaration = declaration;
}

void SelectClauseBuilder::setSynonym(std::string synonym) {
    selectClause.synonym = synonym;
}

SelectClause SelectClauseBuilder::getClause() {
    return selectClause;
}
