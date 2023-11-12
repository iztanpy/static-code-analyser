#include "qps/clauses/pattern_clauses/while.h"

Constraint WhilePattern::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg) {
    return Constraint{WhileEvaluator::Handle(this->declaration.synonym, lhs_arg, pkb_reader)};
  }, lhs);
}

void WhilePattern::Validate() {
  PatternValidator::ValidateWhile(declaration, lhs);
}
