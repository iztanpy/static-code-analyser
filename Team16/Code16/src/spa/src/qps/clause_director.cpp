#include <vector>
#include "qps/clause_director.h"
#include "qps/select_clause.h"
#include "qps/select_clause_builder.h"
#include "qps/declaration.h"

SelectClause ClauseDirector::makeSelectClause(
        SelectClauseBuilder builder, const QueryToken& token, const std::vector<Declaration>& declarations
        ) {
    // find the design entity using the synonym in token
    DesignEntity design_entity;
    for (const Declaration& declaration : declarations) {
        if (declaration.synonym == token.text) {
            design_entity = declaration.design_entity;
            break;
        }
    }
    builder.setSynonym(token.text);
    builder.setEntity(design_entity);
    return builder.getClause();
}

bool ClauseDirector::dummyMethod() {
    return true;
}
