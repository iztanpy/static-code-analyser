#pragma once

#include <string>
#include "qps/clauses/select_clause.h"
#include "qps/query_parser/clause_builder/clause_builder.h"

class SelectClauseBuilder : public ClauseBuilder {
 public:
  SelectClauseBuilder();
  SelectClause selectClause;
  void setEntity(DesignEntity entity);
  void setSynonym(std::string synonym);
  SelectClause getClause() const;
};
