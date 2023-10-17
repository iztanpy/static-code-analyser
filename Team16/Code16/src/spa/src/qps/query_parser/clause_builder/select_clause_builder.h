#pragma once

#include <string>
#include <memory>
#include <utility>

#include "qps/clauses/select_clause.h"
#include "qps/query_parser/clause_builder/clause_builder.h"

class SelectClauseBuilder : public ClauseBuilder {
 public:
  SelectClauseBuilder();
  SelectClause selectClause;

  void setEntity(DesignEntity entity);

  /*!
   * Setts the Declaration synonym of the select clause
   * @param declaration Declaration of the variable
   */
  void setDeclaration(Declaration declaration);

  /*!
   * Returns the select clause
   * @return the select clause
   */
  std::unique_ptr<SelectClause> getClause() const;
};
