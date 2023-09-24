#pragma once

#include <string>
#include <unordered_set>

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "qps/declaration.h"
#include "PKB/API/ReadFacade.h"
#include "evaluator_util.h"
#include "utils/clauses_types.h"

struct AssignPatternEvaluator {
  // TODO(phuccuongngo99): Please mint this to a separate type please
  // like the partial match part
  static BinaryConstraint Handle(std::string& assign_synonym,
                                 Declaration& lhs,
                                 std::string& partial_match,
                                 ReadFacade& pkb_reader);

  static BinaryConstraint Handle(std::string& assign_synonym, Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& assign_synonym,
                                Wildcard& lhs,
                                std::string& partial_match,
                                ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& assign_synonym, Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& assign_synonym,
                                std::string& lhs,
                                std::string& partial_match,
                                ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& assign_synonym, std::string& lhs, Wildcard& rhs, ReadFacade& pkb_reader);
};
