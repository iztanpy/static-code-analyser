#include "qps/clauses/suchthat_clause.h"

#include <utility>

// Checks if both RefParam are of type StmtRef
bool SuchThatClause::are_stmt_ref(const RefParam& param_1, const RefParam& param_2) {
  return std::holds_alternative<StmtRef>(param_1) && std::holds_alternative<StmtRef>(param_2);
}

// Checks if both RefParam are of type EntRef
bool SuchThatClause::are_ent_ref(const RefParam& param_1, const RefParam& param_2) {
  return std::holds_alternative<EntRef>(param_1) && std::holds_alternative<EntRef>(param_2);
}

// Checks if both StmtRef are of type Declaration
bool SuchThatClause::are_stmt_decl(const StmtRef& param_1, const StmtRef& param_2) {
  return std::holds_alternative<Declaration>(param_1) && std::holds_alternative<Declaration>(param_2);
}

// Checks if both EntRef are of type Declaration
bool SuchThatClause::are_ent_decl(const EntRef& param_1, const EntRef& param_2) {
  return std::holds_alternative<Declaration>(param_1) && std::holds_alternative<Declaration>(param_2);
}

// Checks if both StmtRef are of type Wildcard
bool SuchThatClause::are_stmt_wildcard(const StmtRef& param_1, const StmtRef& param_2) {
  return std::holds_alternative<Wildcard>(param_1) && std::holds_alternative<Wildcard>(param_2);
}

// Checks if both EntRef are of type Wildcard
bool SuchThatClause::are_ent_wildcard(const EntRef& param_1, const EntRef& param_2) {
  return std::holds_alternative<Wildcard>(param_1) && std::holds_alternative<Wildcard>(param_2);
}

bool SuchThatClause::are_stmt_ref_equal(const RefParam& param_1, const RefParam& param_2) {
  // Check that both are StmtRef
  if (are_stmt_ref(param_1, param_2)) {
    // Extract the StmtRef
    StmtRef stmt_ref_1 = std::get<StmtRef>(param_1);
    StmtRef stmt_ref_2 = std::get<StmtRef>(param_2);
    if (are_stmt_decl(stmt_ref_1, stmt_ref_2)) {
      Declaration decl_1 = std::get<Declaration>(stmt_ref_1);
      Declaration decl_2 = std::get<Declaration>(stmt_ref_2);
      return decl_1.equals(decl_2);
    } else if (are_stmt_wildcard(stmt_ref_1, stmt_ref_2)) {
      return true;
    } else {
      return std::get<int>(stmt_ref_1) == std::get<int>(stmt_ref_2);
    }
  } else {
    return false;
  }
}

bool SuchThatClause::are_ent_ref_equal(const RefParam& param_1, const RefParam& param_2) {
  // Check that both are EntRef
  if (are_ent_ref(param_1, param_2)) {
    // Extract the EntRef
    EntRef ent_ref_1 = std::get<EntRef>(param_1);
    EntRef ent_ref_2 = std::get<EntRef>(param_2);
    if (are_ent_decl(ent_ref_1, ent_ref_2)) {
      Declaration decl_1 = std::get<Declaration>(ent_ref_1);
      Declaration decl_2 = std::get<Declaration>(ent_ref_2);
      return decl_1.equals(decl_2);
    } else if (are_ent_wildcard(ent_ref_1, ent_ref_2)) {
      return true;
    } else {
      return std::get<std::string>(ent_ref_1) == std::get<std::string>(ent_ref_2);
    }
  } else {
    return false;
  }
}

UsesS::UsesS(StmtRef lhs, EntRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint UsesS::Evaluate() {
  throw QpsSemanticError("Not implemented");
}

UsesP::UsesP(EntRef lhs, EntRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint UsesP::Evaluate() {
  throw QpsSemanticError("Not implemented");
}

ModifiesP::ModifiesP(EntRef lhs, EntRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint ModifiesP::Evaluate() {
  throw QpsSemanticError("Not implemented");
}

ModifiesS::ModifiesS(StmtRef lhs, EntRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint ModifiesS::Evaluate() {
  throw QpsSemanticError("Not implemented");
}

Follows::Follows(StmtRef lhs, StmtRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint Follows::Evaluate() {
  throw QpsSemanticError("Not implemented");
}

FollowsT::FollowsT(StmtRef lhs, StmtRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint FollowsT::Evaluate() {
  throw QpsSemanticError("Not implemented");
}

Parent::Parent(StmtRef lhs, StmtRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint Parent::Evaluate() {
  throw QpsSemanticError("Not implemented");
}

ParentS::ParentS(StmtRef lhs, StmtRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint ParentS::Evaluate() {
  throw QpsSemanticError("Not implemented");
}
