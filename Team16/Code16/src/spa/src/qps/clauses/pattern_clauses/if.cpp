#include "qps/clauses/pattern_clauses/if.h"

Constraint IfPattern::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg) {
    return Constraint{IfEvaluator::Handle(this->declaration.synonym, lhs_arg, pkb_reader)};
  }, lhs);
}

void IfPattern::Validate() {
  PatternValidator::ValidateIf(declaration, lhs);
}
