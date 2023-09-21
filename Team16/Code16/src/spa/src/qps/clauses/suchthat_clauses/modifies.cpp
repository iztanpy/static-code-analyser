#include "qps/clauses/suchthat_clauses/modifies.h"

Constraint ModifiesP::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("Not implemented");
}

void ModifiesP::Validate() {
  SuchThatValidator::ValidateModifies(lhs, rhs);
}

Constraint ModifiesS::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("Not implemented");
}
void ModifiesS::Validate() {
  SuchThatValidator::ValidateModifies(lhs, rhs);
}
