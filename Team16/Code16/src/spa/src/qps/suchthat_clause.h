#pragma once

#include <string>
#include <variant>

#include "qps/declaration.h"
#include "qps/clause.h"

enum class RelRef {
  Follows,
  FollowsT,
  Parent,
  ParentT,
  UsesS,
  UsesP,
  ModifiesS,
  ModifiesP,
};

enum class Underscore {
  Value
};

using IntegerIdent = std::variant<int, std::string>;
using ParamRef = std::variant<Declaration, Underscore, IntegerIdent>;

/**
 * Represents a such that clause in a parsed query. Query Evaluator
 * will use this to build an Evaluator object. Evaluator will then
 * call pkb to get back valid values
 */
class SuchThatClause : public Clause {
 public:
  SuchThatClause(RelRef rel_ref, ParamRef& lhs, ParamRef& rhs);

 private:
  RelRef rel_ref;
  ParamRef lhs;
  ParamRef rhs;
};
