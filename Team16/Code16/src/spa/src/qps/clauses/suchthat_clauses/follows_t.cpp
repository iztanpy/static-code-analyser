#include "qps/clauses/suchthat_clauses/follows_t.h"

FollowsT::FollowsT(StmtRef lhs, StmtRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint FollowsT::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("Not implemented");
}
