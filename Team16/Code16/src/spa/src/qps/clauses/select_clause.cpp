#include "qps/clauses/select_clause.h"

bool SelectClause::equals(const SelectClause& other) const {
  return this->declaration.equals(other.declaration);
}

UnaryConstraint SelectClause::Evaluate(ReadFacade& pkb_reader) {
  return SelectEvaluator::Evaluate(declaration, pkb_reader);
}
