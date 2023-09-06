#pragma once

#include <string>
#include "SelectClause.h"
#include "ClauseBuilder.h"

class SelectClauseBuilder: public ClauseBuilder {
 public:
    SelectClause selectClause;
    void setDeclaration(std::string declaration);
    void setSynonym(std::string synonym);
    /* TODO: set such that and pattern clauses */
    SelectClause getClause();
    bool validateClause() override{
        return true;
    }
};
