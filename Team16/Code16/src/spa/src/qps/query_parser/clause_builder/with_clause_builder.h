#pragma once

#include <memory>
#include "utils/clauses_types.h"
#include "qps/query_parser/clause_builder/clause_builder.h"
#include "qps/clauses/with_clause.h"

class WithClauseBuilder : public ClauseBuilder {
 public:
  WithClauseBuilder();
  Ref lhs;
  Ref rhs;

  /*!
   * Sets the lhs Ref of the with clause
   * @param lhs is the Ref to be set
   */
  void setLhs(Ref lhs);

  /*!
   * Sets the rhs Ref of the with clause
   * @param rhs is the Ref to be set
   */
  void setRhs(Ref rhs);

  /*!
   * Returns a pointer to the with clause
   * @return a pointer to the with clause
   */
  std::unique_ptr<WithClause> getClause() const;
};
