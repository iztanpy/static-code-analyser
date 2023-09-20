#include "qps/clauses/suchthat_clauses/uses_p.h"

UsesP::UsesP(EntRef lhs, EntRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint UsesP::Evaluate(ReadFacade& pkb_reader) {
  throw QpsSemanticError("Not implemented");
}
