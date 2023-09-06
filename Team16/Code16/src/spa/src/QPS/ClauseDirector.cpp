//
// Created by Thia Su Mian on 4/9/23.
//

#include "ClauseDirector.h"
#include "SelectClause.h"
#include "SelectClauseBuilder.h"

SelectClause ClauseDirector::makeSelectClause(SelectClauseBuilder builder, std::string query) {
    builder.setDeclaration(query);
    builder.setKeyword(query);
    builder.setSynonym(query);
    /* TODO: set such that and pattern clauses */
}

bool ClauseDirector::dummyMethod() {
    return true;
}
