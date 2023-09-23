#pragma once

#include <string>
#include "qps/clauses/select_clause.h"
#include "qps/query_parser/clause_builder/clause_builder.h"

class SelectClauseBuilder : public ClauseBuilder {
 public:
  SelectClauseBuilder();
  SelectClause selectClause;

  /*!
   * Setts the design entity of the select clause
   * @param entity Design entity of the variable
   */
  void setEntity(DesignEntity entity);

  /*!
   * Sets the synonym of the select clause
   * @param synonym is the string synonym
   */
  void setSynonym(std::string synonym);

  /*!
   * Returns the select clause
   * @return the select clause
   */
  SelectClause getClause() const;
};
