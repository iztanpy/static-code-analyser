#pragma once

#include "qps/clauses/pattern_clauses/pattern_clause.h"
#include "qps/query_evaluator/assign_pattern_evaluator.h"

/*!
 * Represents a Assign clause in a query according to PQL grammar
 */
class AssignPattern : public PatternClause {
 public:
  ExprSpec rhs;

  RelRefType GetRelRef() const override {
    return RelRefType::ASSIGN;
  }

  AssignPattern(Declaration syn, EntRef lhs, ExprSpec rhs, bool is_not)
      : PatternClause(syn, lhs, is_not), rhs(rhs) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

  bool equals(const Clause* other) const override;

  // Overloaded == operator
  friend bool operator==(const AssignPattern& lhs, const AssignPattern& rhs);

  /*!
   * Functions to support hashing of assign pattern
   * because it has an extra field ExprSpec
   */
  size_t Hash() const override;

 private:
  void Validate() override;
};
