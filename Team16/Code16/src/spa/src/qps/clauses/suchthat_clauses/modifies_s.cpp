#include "qps/clauses/suchthat_clauses/modifies_s.h"

ModifiesS::ModifiesS(StmtRef lhs, EntRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint ModifiesS::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("Not implemented");
}
