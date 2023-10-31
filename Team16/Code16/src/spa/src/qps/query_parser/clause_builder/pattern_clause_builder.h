#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <string>

#include "qps/query_parser/clause_builder/clause_builder.h"
#include "qps/declaration.h"
#include "qps/clauses/pattern_clauses/pattern_clause.h"
#include "utils/entity_types.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"

enum PatternType {
  kIf,
  kWhile,
  kAssignSyn
};

class PatternClauseBuilder : public ClauseBuilder {
 public:
  PatternType pattern_type;
  Declaration syn_assignment;
  EntRef lhs;
  ExprSpec rhs;
  bool is_not;

  /*!
   * Sets the pattern type and syn_assignment of the pattern clause
   * @param token first token from pattern clause
   * @param declarations is the set of declarations
   */
  void setPatternType(const QueryToken& token, const std::vector<Declaration>& declarations);

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
