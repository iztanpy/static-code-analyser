#include "qps/clauses/suchthat_clauses/follows.h"

Constraint Follows::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("Not implemented");
}
void Follows::Validate() {
  SuchThatValidator::ValidateParentFollows(lhs, rhs);
}

Constraint FollowsT::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("Not implemented");
}

void FollowsT::Validate() {
  SuchThatValidator::ValidateParentFollows(lhs, rhs);
}
