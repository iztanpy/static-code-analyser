#pragma once

#include <memory>
#include <vector>
#include "utils/clauses_types.h"
#include "qps/query_parser/clause_builder/clause_builder.h"
#include "qps/clauses/with_clauses/with_clause.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"

class WithClauseBuilder : public ClauseBuilder {
 public:
  WithClauseBuilder();
  Ref lhs;
  Ref rhs;

  /*!
   * Sets the lhs Ref of the with clause
   * @param lhs is the Ref to be set
   * @param declarations is the set of declarations
   */
  void setLhs(const QueryToken& param, const std::vector<Declaration>& declarations);

  /*!
   * Sets the rhs Ref of the with clause
   * @param rhs is the Ref to be set
   * @param declarations is the set of declarations
   */
  void setRhs(const QueryToken& param, const std::vector<Declaration>& declarations);

  /*!
   * Returns a pointer to the with clause
   * @return a pointer to the with clause
   */
  std::unique_ptr<WithClause> getClause() const;
};
