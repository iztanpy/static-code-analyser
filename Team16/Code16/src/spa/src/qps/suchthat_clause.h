#pragma once

#include <string>

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

class SuchThatClause : public Clause {
 public:
  SuchThatClause(RelRef rel_ref, ParamRef& lhs, ParamRef& rhs);

 private:
  RelRef rel_ref;
  ParamRef lhs;
  ParamRef rhs;
};
