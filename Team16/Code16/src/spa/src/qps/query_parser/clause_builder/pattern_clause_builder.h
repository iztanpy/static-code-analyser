#pragma once

#include <vector>
#include <memory>
#include "qps/query_parser/clause_builder/clause_builder.h"
#include "qps/declaration.h"
#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/pattern_clauses/pattern_clause.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"

class PatternClauseBuilder : public ClauseBuilder {
 public:
  Declaration syn_assignment;
  EntRef lhs;
  ExprSpec rhs;
  PQLTokenType rhs_type;  // indicator to know what whether rhs is expr or sub expr

  /*!
   * Sets the synonym assignment of pattern clause
   * @param declaration the set of declarations
   */
  void setSynAssignment(Declaration declaration);

  /*!
   * Sets the left argument of the pattern clause
   * @param param is the query token
   * @param declarations is the set of declarations
   */
  void setLhs(const QueryToken& param, const std::vector<Declaration>& declarations);

  /*!
   * Sets the right argument of the pattern clause
   * @param param is the query token
   * @param declarations is the set of declarations
   */
  void setRhs(const QueryToken& param, const std::vector<Declaration>& declarations);

  /*!
   * Returns a pointer to the pattern clause
   * @return a pointer to the pattern clause
   */
  std::unique_ptr<PatternClause> getClause() const;
};
