#pragma once

#include <string>
#include <memory>
#include <utility>

#include "qps/clauses/select_clause.h"
#include "qps/query_parser/clause_builder/clause_builder.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"

class SelectClauseBuilder : public ClauseBuilder {
 public:
  SelectClauseBuilder();
  SelectClause selectClause;

  /*!
   * Setts the Declaration synonym of the select clause
   * @param declaration Declaration of the variable
   * @param token_type the token type of select argument
   */
  void setDeclaration(Declaration declaration, PQLTokenType token_type);

  /*!
   * Returns the select clause
   * @return the select clause
   */
  std::unique_ptr<SelectClause> getClause() const;
};
