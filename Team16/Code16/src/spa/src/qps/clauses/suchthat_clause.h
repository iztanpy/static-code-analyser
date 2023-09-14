#pragma once

#include <string>
#include <variant>

#include "qps/declaration.h"
#include "qps/clauses/clause.h"
#include "qps/rel_ref.h"
#include "qps/query_evaluator/constraint.h"

enum class Wildcard {
  Value
};

using StmtRef = std::variant<Declaration, Wildcard, int>;
using EntRef = std::variant<Declaration, Wildcard, std::string>;
using RefParam = std::variant<StmtRef, EntRef>;

/**
 * Represents a such that clause in a parsed query. Query Evaluator
 * will use this to build an Evaluator object. Evaluator will then
 * call PKB to get back valid values
 */
class SuchThatClause : public Clause {
 public:
  // TODO(cuong): virtual Constraint Evaluate() = 0;
  ~SuchThatClause() override = default;
  RelRefType rel_ref;  // an identifier to make things easier
  RefParam lhs;
  RefParam rhs;

  static bool are_stmt_ref_equal(const RefParam & param_1, const RefParam & param_2);
  static bool are_ent_ref_equal(const RefParam & param_1, const RefParam & param_2);
  static bool are_stmt_ref(const RefParam & param_1, const RefParam & param_2);
  static bool are_ent_ref(const RefParam & param_1, const RefParam & param_2);
  static bool are_stmt_decl(const StmtRef & param_1, const StmtRef & param_2);
  static bool are_ent_decl(const EntRef & param_1, const EntRef & param_2);
  static bool are_stmt_wildcard(const StmtRef & param_1, const StmtRef & param_2);
  static bool are_ent_wildcard(const EntRef & param_1, const EntRef & param_2);
};

class UsesP : public SuchThatClause {
 public:
  UsesP(EntRef lhs, EntRef rhs);
  EntRef lhs;
  EntRef rhs;
};

class UsesS : public SuchThatClause {
 public:
  UsesS(StmtRef lhs, EntRef rhs);
  StmtRef lhs;
  EntRef rhs;
};

class ModifiesP : public SuchThatClause {
 public:
  ModifiesP(EntRef lhs, EntRef rhs);
  EntRef lhs;
  EntRef rhs;
};

class ModifiesS : public SuchThatClause {
 public:
  ModifiesS(StmtRef lhs, EntRef rhs);
  StmtRef lhs;
  EntRef rhs;
};

class Follows : public SuchThatClause {
 public:
  Follows(StmtRef lhs, StmtRef rhs);
  StmtRef lhs;
  StmtRef rhs;
};

class FollowsT : public SuchThatClause {
 public:
  FollowsT(StmtRef lhs, StmtRef rhs);
  StmtRef lhs;
  StmtRef rhs;
};

class Parent : public SuchThatClause {
 public:
  Parent(StmtRef lhs, StmtRef rhs);
  StmtRef lhs;
  StmtRef rhs;
};

class ParentS : public SuchThatClause {
 public:
  ParentS(StmtRef lhs, StmtRef rhs);
  StmtRef lhs;
  StmtRef rhs;
};
