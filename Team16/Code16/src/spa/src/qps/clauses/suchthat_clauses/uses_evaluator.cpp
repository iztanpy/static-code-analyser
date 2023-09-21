#include "qps/clauses/suchthat_clauses/uses_evaluator.h"

UnaryConstraint UsesEvaluator::Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader) {
  std::unordered_set<std::string> result = pkb_reader.getVariablesUsedBy(lhs);
  return UnaryConstraint{rhs.synonym, result};
}

bool UsesEvaluator::Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader) {
  throw QpsSemanticError("[Uses] Not implemented");
}

bool UsesEvaluator::Handle(int lhs, std::string& rhs, ReadFacade& pkb_reader) {
  throw QpsSemanticError("[Uses] Not implemented");
}

BinaryConstraint UsesEvaluator::Handle(Declaration& lhs,
                                       Declaration& rhs,
                                       ReadFacade& pkb_reader) {
  throw QpsSemanticError("[Uses] Not implemented");
}

UnaryConstraint UsesEvaluator::Handle(Declaration& lhs,
                                      Wildcard& rhs,
                                      ReadFacade& pkb_reader) {
  throw QpsSemanticError("[Uses] Not implemented");
}

UnaryConstraint UsesEvaluator::Handle(Declaration& lhs,
                                      std::string& rhs,
                                      ReadFacade& pkb_reader) {
  throw QpsSemanticError("[Uses] Not implemented");
}

UnaryConstraint UsesEvaluator::Handle(Wildcard& lhs,
                                      Declaration& rhs,
                                      ReadFacade& pkb_reader) {
  assert(false && "[Uses] This code should never be reached! Make sure to call Validate() within constructor");
  return UnaryConstraint{rhs.synonym, {}};
}

bool UsesEvaluator::Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader) {
  assert(false && "[Uses] This code should never be reached! Make sure to call Validate() within constructor");
  return false;
}

bool UsesEvaluator::Handle(Wildcard& lhs, std::string& rhs, ReadFacade& pkb_reader) {
  assert(false && "[Uses] This code should never be reached! Make sure to call Validate() within constructor");
  return false;
}

UnaryConstraint UsesEvaluator::Handle(std::string& lhs_procname,
                                      Declaration& rhs,
                                      ReadFacade& pkb_reader) {
  throw QpsSemanticError("[Uses] Not required by Milestone1");
}

bool UsesEvaluator::Handle(std::string& lhs_proc_name, Wildcard& rhs, ReadFacade& pkb_reader) {
  throw QpsSemanticError("[Uses] Not required by Milestone1");
}

bool UsesEvaluator::Handle(std::string& lhs_proc_name, std::string& rhs, ReadFacade& pkb_reader) {
  throw QpsSemanticError("[Uses] Not required by Milestone1");
}
