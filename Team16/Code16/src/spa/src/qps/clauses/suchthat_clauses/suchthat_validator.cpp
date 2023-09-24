#include "qps/clauses/suchthat_clauses/suchthat_validator.h"

void SuchThatValidator::ValidateParentFollows(StmtRef lhs, StmtRef rhs) {
  if (std::holds_alternative<Declaration>(lhs)) {
    Declaration lhs_decl = std::get<Declaration>(lhs);
    if (kParentFollowsValidRhsLhs.find(lhs_decl.design_entity) == kParentFollowsValidRhsLhs.end()) {
      throw QpsSemanticError("[ParentFollows] Invalid LHS synonym");
    }
  }

  if (std::holds_alternative<Declaration>(rhs)) {
    Declaration rhs_decl = std::get<Declaration>(rhs);
    if (kParentFollowsValidRhsLhs.find(rhs_decl.design_entity) == kParentFollowsValidRhsLhs.end()) {
      throw QpsSemanticError("[ParentFollows] Invalid RHS synonym");
    }
  }
}

void SuchThatValidator::ValidateUsesModifies(EntRef lhs, EntRef rhs) {
  if (std::holds_alternative<Wildcard>(lhs)) {
    throw QpsSemanticError("[Uses/Modifies] LHS cannot be wildcard");
  }

  if (std::holds_alternative<Declaration>(lhs)) {
    Declaration lhs_decl = std::get<Declaration>(lhs);
    if (kUsesModifiesValidLhs.find(lhs_decl.design_entity) == kUsesModifiesValidLhs.end()) {
      throw QpsSemanticError("[Uses/Modifies] Invalid LHS synonym");
    }
  }

  if (std::holds_alternative<Declaration>(rhs)) {
    Declaration rhs_decl = std::get<Declaration>(rhs);
    if (kUsesModifiesValidRhs.find(rhs_decl.design_entity) == kUsesModifiesValidRhs.end()) {
      throw QpsSemanticError("[Uses/Modifies] Invalid RHS synonym");
    }
  }
}

void SuchThatValidator::ValidateUsesModifies(StmtRef lhs, EntRef rhs) {
  if (std::holds_alternative<Wildcard>(lhs)) {
    throw QpsSemanticError("[Uses/Modifies] LHS cannot be wildcard");
  }

  if (std::holds_alternative<Declaration>(lhs)) {
    Declaration lhs_decl = std::get<Declaration>(lhs);
    if (kUsesModifiesValidLhs.find(lhs_decl.design_entity) == kUsesModifiesValidLhs.end()) {
      throw QpsSemanticError("[Uses/Modifies] Invalid LHS synonym");
    }
  }

  if (std::holds_alternative<Declaration>(rhs)) {
    Declaration rhs_decl = std::get<Declaration>(rhs);
    if (kUsesModifiesValidRhs.find(rhs_decl.design_entity) == kUsesModifiesValidRhs.end()) {
      throw QpsSemanticError("[Uses/Modifies] Invalid RHS synonym");
    }
  }
}
