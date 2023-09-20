#include "qps/clauses/suchthat_clauses/modifies_p.h"

ModifiesP::ModifiesP(EntRef lhs, EntRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint ModifiesP::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("Not implemented");
}
