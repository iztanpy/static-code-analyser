#pragma once

#include <string>
#include <unordered_set>
#include <cassert>
#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/evaluator_util.h"

struct UsesEvaluator {
  static UnaryConstraint Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader);

  static bool Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader);

  static bool Handle(int lhs, std::string& rhs, ReadFacade& pkb_reader);

  static Constraint Handle(Declaration& lhs,
                           Declaration& rhs,
                           ReadFacade& pkb_reader);

  static UnaryConstraint Handle(Declaration& lhs,
                                Wildcard& rhs,
                                ReadFacade& pkb_reader);

  static UnaryConstraint Handle(Declaration& lhs,
                                std::string& rhs,
                                ReadFacade& pkb_reader);

  static UnaryConstraint Handle(Wildcard& lhs,
                                Declaration& rhs,
                                ReadFacade& pkb_reader);

  static bool Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader);

  static bool Handle(Wildcard& lhs, std::string& rhs, ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& lhs_procname,
                                Declaration& rhs,
                                ReadFacade& pkb_reader);

  static bool Handle(std::string& lhs_proc_name, Wildcard& rhs, ReadFacade& pkb_reader);

  static bool Handle(std::string& lhs_proc_name, std::string& rhs, ReadFacade& pkb_reader);
};
