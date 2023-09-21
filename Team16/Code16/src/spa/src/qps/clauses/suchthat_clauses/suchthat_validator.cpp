#include "qps/clauses/suchthat_clauses/suchthat_validator.h"

void SuchThatValidator::ValidateParentFollow(StmtRef lhs, StmtRef rhs) {
  if (std::holds_alternative<Declaration>(lhs)) {
    Declaration lhs_decl = std::get<Declaration>(lhs);
    if (kParentFollowsValidRhsLhs.find(lhs_decl.design_entity) == kParentFollowsValidRhsLhs.end()) {
      throw QpsSemanticError("[Parent] Invalid LHS synonym");
    }
  }

  if (std::holds_alternative<Declaration>(rhs)) {
    Declaration rhs_decl = std::get<Declaration>(rhs);
    if (kParentFollowsValidRhsLhs.find(rhs_decl.design_entity) == kParentFollowsValidRhsLhs.end()) {
      throw QpsSemanticError("[Parent] Invalid RHS synonym");
    }
  }
}

void SuchThatValidator::ValidateUse(EntRef lhs, EntRef rhs) {
  if (std::holds_alternative<Declaration>(lhs)) {
    Declaration lhs_decl = std::get<Declaration>(lhs);
    if (kUsesValidLhs.find(lhs_decl.design_entity) == kUsesValidLhs.end()) {
      throw QpsSemanticError("[Uses] Invalid LHS synonym");
    }
  }

  if (std::holds_alternative<Wildcard>(lhs)) {
    throw QpsSemanticError("[Uses] LHS cannot be wildcard");
  }

  if (std::holds_alternative<Declaration>(rhs)) {
    Declaration rhs_decl = std::get<Declaration>(rhs);
    if (kUsesValidRhs.find(rhs_decl.design_entity) == kUsesValidRhs.end()) {
      throw QpsSemanticError("[Uses] Invalid RHS synonym");
    }
  }
}

void SuchThatValidator::ValidateUse(StmtRef lhs, EntRef rhs) {
  if (std::holds_alternative<Declaration>(lhs)) {
    Declaration lhs_decl = std::get<Declaration>(lhs);
    if (kUsesValidLhs.find(lhs_decl.design_entity) == kUsesValidLhs.end()) {
      throw QpsSemanticError("[Uses] Invalid LHS synonym");
    }
  }

  if (std::holds_alternative<Wildcard>(lhs)) {
    throw QpsSemanticError("[Uses] LHS cannot be wildcard");
  }

  if (std::holds_alternative<Declaration>(rhs)) {
    Declaration rhs_decl = std::get<Declaration>(rhs);
    if (kUsesValidRhs.find(rhs_decl.design_entity) == kUsesValidRhs.end()) {
      throw QpsSemanticError("[Uses] Invalid RHS synonym");
    }
  }
}
