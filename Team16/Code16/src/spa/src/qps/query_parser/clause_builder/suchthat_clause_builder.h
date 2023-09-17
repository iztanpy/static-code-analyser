#pragma once

#include <string>
#include <memory>
#include <vector>
#include "qps/clauses/suchthat_clause.h"
#include "qps/query_parser/clause_builder/clause_builder.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"

class SuchThatClauseBuilder : public ClauseBuilder {
 public:
  SuchThatClauseBuilder();
  RelRefType rel_ref;
  RefParam lhs;
  RefParam rhs;
  void setRelRef(RelRefType relRef);
  void setLhs(const QueryToken & param, const std::vector<Declaration> & declarations);
  void setRhs(const QueryToken & param, const std::vector<Declaration> & declarations);
  static RefParam getStmtRef(const QueryToken & param, const std::vector<Declaration> & declarations);
  static RefParam getEntRef(const QueryToken & param, const std::vector<Declaration> & declarations);
  static RefParam getBothStmtAndEntRef(const QueryToken & param, const std::vector<Declaration> & declarations);
  std::unique_ptr<SuchThatClause> getClause() const;
};
