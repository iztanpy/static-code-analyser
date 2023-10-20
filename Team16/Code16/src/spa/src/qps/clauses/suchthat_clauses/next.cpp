#include "qps/clauses/suchthat_clauses/next.h"

Constraint Next::Evaluate(ReadFacade& pkb_reader) {
  return {};
}

void Next::Validate() {
  SuchThatValidator::ValidateBothStmt(lhs, rhs);
}

Constraint NextT::Evaluate(ReadFacade& pkb_reader) {
  return {};
}

void NextT::Validate() {
  SuchThatValidator::ValidateBothStmt(lhs, rhs);
}
