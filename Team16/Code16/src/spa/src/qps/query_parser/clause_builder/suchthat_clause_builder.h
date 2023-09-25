#pragma once

#include <string>
#include <memory>
#include <vector>
#include "qps/clauses/suchthat_clauses/suchthat_clauses_all.h"
#include "qps/query_parser/clause_builder/clause_builder.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/qps_errors/qps_syntax_error.h"

class SuchThatClauseBuilder : public ClauseBuilder {
 public:
  SuchThatClauseBuilder();
  RelRefType rel_ref;
  RefParam lhs;
  RefParam rhs;

  /*!
   * Sets the relation reference of such that clause
   * @param relRef is the relation reference
   */
  void setRelRef(RelRefType relRef);

  /*!
   * Sets the left argument of the such that clause
   * @param param is the query token
   * @param declarations is the set of declarations
   */
  void setLhs(const QueryToken& param, const std::vector<Declaration>& declarations);

  /*!
   * Sets the right argument of the such that clause
   * @param param is the query token
   * @param declarations is the set of declarations
   */
  void setRhs(const QueryToken& param, const std::vector<Declaration>& declarations);

  /*!
   * Gets a statement reference based on query token and declarations
   * @param param is the query token
   * @param declarations is the set of declarations
   * @return a statement reference
   */
  static RefParam getStmtRef(const QueryToken& param, const std::vector<Declaration>& declarations);

  /*!
   * Gets an entity reference based on query token and declarations
   * @param param is the query token
   * @param declarations is the set of declarations
   * @return a entity reference
   */
  static RefParam getEntRef(const QueryToken& param, const std::vector<Declaration>& declarations);

  /*!
   * Gets either an entity reference or statement reference based on query token and declarations
   * @param param the query token
   * @param declarations the set of declarations
   * @return a reference parameter that could be either an entity reference or a statement reference
   */
  static RefParam getBothStmtAndEntRef(const QueryToken& param, const std::vector<Declaration>& declarations);

  /*!
   * Returns a pointer to the such that clause
   * @return a pointer to the such that clause
   */
  std::unique_ptr<SuchThatClause> getClause() const;
};
