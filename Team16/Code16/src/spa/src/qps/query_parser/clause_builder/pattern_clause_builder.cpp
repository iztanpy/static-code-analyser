#pragma once

#include "pattern_clause_builder.h"

void PatternClauseBuilder::setSynAssignment(Declaration declaration) {
  declaration = declaration;
}
void PatternClauseBuilder::setLhs(const QueryToken & param, const std::vector<Declaration> & declarations) {

}
void PatternClauseBuilder::setRhs(const QueryToken & param, const std::vector<Declaration> & declarations) {

}
std::unique_ptr<PatternClause> PatternClauseBuilder::getClause() const {
  return std::unique_ptr<PatternClause>();
}
