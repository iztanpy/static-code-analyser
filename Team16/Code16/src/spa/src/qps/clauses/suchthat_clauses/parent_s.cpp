#include "qps/clauses/suchthat_clauses/parent_s.h"

ParentS::ParentS(StmtRef lhs, StmtRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint ParentS::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("Not implemented");
}
