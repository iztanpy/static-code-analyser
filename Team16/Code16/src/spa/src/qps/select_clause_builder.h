#pragma once

#include <string>
#include "qps/select_clause.h"
#include "qps/clause_builder.h"

class SelectClauseBuilder: public ClauseBuilder {
 public:
    SelectClause selectClause;
    void setEntity(DesignEntity entity);
    void setSynonym(std::string synonym);
    SelectClause& getClause() override;
};
