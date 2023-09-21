#include "qps/clauses/suchthat_clauses/follows.h"

Follows::Follows(StmtRef lhs, StmtRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint Follows::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("Not implemented");
}
void Follows::Validate() {
}
