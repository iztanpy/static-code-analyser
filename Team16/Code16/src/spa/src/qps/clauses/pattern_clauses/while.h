#pragma once

#include "qps/clauses/pattern_clauses/pattern_clause.h"
#include "qps/query_evaluator/while_evaluator.h"

/*!
 * Represents a While clause in a query according to PQL grammar
 */
class WhilePattern : public PatternClause {
 public:
  WhilePattern(Declaration syn, EntRef lhs, bool is_not) : PatternClause(syn, lhs, is_not) {
    Validate();
  }

  RelRefType GetRelRef() const override {
    return RelRefType::WHILE;
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
