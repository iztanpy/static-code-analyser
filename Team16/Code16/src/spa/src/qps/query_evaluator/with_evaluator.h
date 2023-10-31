#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cassert>

#include "utils/algorithm_utils.h"
#include "qps/clauses/attr_ref.h"

/*!
 * Evaluator visitor that handle all combinations of parameters for WithClause
 */
struct WithEvaluator {
  static bool Handle(int lhs, int rhs, ReadFacade& pkb_reader);

  static bool Handle(std::string& lhs, std::string& rhs, ReadFacade& pkb_reader);

  static bool Handle(int integer, std::string& string, ReadFacade& pkb_reader);

  static bool Handle(std::string& string, int integer, ReadFacade& pkb_reader);

  static UnaryConstraint Handle(AttrRef& attr, int integer, ReadFacade& pkb_reader);

  static UnaryConstraint Handle(int integer, AttrRef& attr, ReadFacade& pkb_reader);

  static UnaryConstraint Handle(AttrRef& attr, std::string& string, ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& string, AttrRef& attr, ReadFacade& pkb_reader);

  static Constraint Handle(AttrRef& lhs, AttrRef& rhs, ReadFacade& pkb_reader);
};
