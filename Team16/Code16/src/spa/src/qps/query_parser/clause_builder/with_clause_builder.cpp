#include "with_clause_builder.h"

WithClauseBuilder::WithClauseBuilder() = default;

void WithClauseBuilder::setLhs(Ref lhs) {
  this->lhs = std::move(lhs);
}

void WithClauseBuilder::setRhs(Ref rhs) {
  this->rhs = std::move(rhs);
}

std::unique_ptr<WithClause> WithClauseBuilder::getClause() const {
  return std::make_unique<WithClause>(lhs, rhs);
}

