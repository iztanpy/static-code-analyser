#include "qps/clauses/pattern_validator.h"

void PatternValidator::Validate(Declaration& leading, EntRef& lhs) {
  if (kValidLeading.find(leading.design_entity) == kValidLeading.end()) {
    throw QpsSemanticError("[AssignPattern] syn-assign is not an assign synonym");
  }

  if (std::holds_alternative<Declaration>(lhs)) {
    Declaration lhs_decl = std::get<Declaration>(lhs);
    if (kValidLhs.find(lhs_decl.design_entity) == kValidLhs.end()) {
      throw QpsSemanticError("[AssignPattern] Invalid LHS synonym. Must be VARIABLE synonym");
    }
  }
}
