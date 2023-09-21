#include "qps/clauses/suchthat_clauses/parent.h"

Parent::Parent(StmtRef lhs, StmtRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint Parent::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("Not implemented");
}

void Parent::Validate() {
}
