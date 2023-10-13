#include "qps/clauses/suchthat_clauses/calls.h"

Constraint Calls::Evaluate(ReadFacade& pkb_reader) {
  return {};
}

void Calls::Validate() {
  SuchThatValidator::ValidateCalls(lhs, rhs);
}

Constraint CallsT::Evaluate(ReadFacade& pkb_reader) {
  return {};
}

void CallsT::Validate() {
  SuchThatValidator::ValidateCalls(lhs, rhs);
}
