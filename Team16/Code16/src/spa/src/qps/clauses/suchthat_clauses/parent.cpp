#include "qps/clauses/suchthat_clauses/parent.h"

Constraint Parent::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("Not implemented");
}

void Parent::Validate() {
  SuchThatValidator::ValidateParentFollow(lhs, rhs);
}

Constraint ParentT::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("Not implemented");
}

void ParentT::Validate() {
  SuchThatValidator::ValidateParentFollow(lhs, rhs);
}
