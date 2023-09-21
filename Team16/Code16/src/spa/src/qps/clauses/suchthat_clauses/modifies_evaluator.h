#pragma once

#pragma once

#include <string>
#include <unordered_set>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

struct ModifiesEvaluator {
  static UnaryConstraint Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader);

  static bool Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader);

  static bool Handle(int lhs, std::string& rhs, ReadFacade& pkb_reader);

  static BinaryConstraint Handle(Declaration& lhs,
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
