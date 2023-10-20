#include "qps/clauses/pattern_clauses/pattern_validator.h"

void PatternValidator::ValidateLhs(EntRef& lhs) {
  if (std::holds_alternative<Declaration>(lhs)) {
    Declaration lhs_decl = std::get<Declaration>(lhs);
    if (lhs_decl.design_entity != kValidLhs) {
      throw QpsSemanticError("[Pattern] Invalid LHS synonym. Must be VARIABLE synonym");
    }
  }
}

void PatternValidator::ValidateAssign(Declaration& leading, EntRef& lhs) {
  if (leading.design_entity != kValidAssign) {
    throw QpsSemanticError("[AssignPattern] syn-assign is not an assign synonym");
  }

  ValidateLhs(lhs);
}

void PatternValidator::ValidateIf(Declaration& leading, EntRef& lhs) {
  if (leading.design_entity != kValidIf) {
    throw QpsSemanticError("[IfPattern] syn-if is not an if synonym");
  }

  ValidateLhs(lhs);
}

void PatternValidator::ValidateWhile(Declaration& leading, EntRef& lhs) {
  if (leading.design_entity != kValidWhile) {
    throw QpsSemanticError("[WhilePattern] syn-while is not a while synonym");
  }

  ValidateLhs(lhs);
}

