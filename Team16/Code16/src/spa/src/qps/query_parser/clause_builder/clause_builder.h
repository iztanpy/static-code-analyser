#pragma once

#include <string>
#include <utility>
#include "qps/clauses/clause.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/qps_errors/qps_syntax_error.h"

class ClauseBuilder {
 public:
  bool is_not;
  /*!
   * Sets the clause to be either a not clause or a normal clause
   * @param param is the query token indicating whether it is a not clause
   */
  void setNot(const QueryToken& param) {
    switch (param.type) {
      case PQLTokenType::NOT_CLAUSE: this->is_not = true;
        break;
      case PQLTokenType::NORMAL_CLAUSE: this->is_not = false;
        break;
      default:
        throw QpsSyntaxError("Syntax Error");
    }
  }

  ClauseBuilder(): is_not(false) {}  // by default, it is a normal clause
  virtual ~ClauseBuilder() = default;
};
