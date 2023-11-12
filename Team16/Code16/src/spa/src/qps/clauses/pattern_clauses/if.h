#pragma once

#include <qps/clauses/pattern_clauses/pattern_clause.h>
#include "qps/query_evaluator/if_evaluator.h"

/*!
 * Represents a If clause in a query according to PQL grammar
 */
class IfPattern : public PatternClause {
 public:
  IfPattern(Declaration syn, EntRef lhs, bool is_not) : PatternClause(syn, lhs, is_not) {
    Validate();
  }

  RelRefType GetRelRef() const override {
    return RelRefType::IF;
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
