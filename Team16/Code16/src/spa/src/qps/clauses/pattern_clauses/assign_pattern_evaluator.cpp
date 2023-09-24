#include "assign_pattern_evaluator.h"

BinaryConstraint AssignPatternEvaluator::Handle(std::string& assign_synonym,
                                                Declaration& lhs,
                                                std::string& partial_match,
                                                ReadFacade& pkb_reader) {
  IntStringPairSet results = pkb_reader.getAssignPair(partial_match);
  return {{assign_synonym, lhs.synonym}, EvaluatorUtil::ToStringPairSet(results)};
}

BinaryConstraint AssignPatternEvaluator::Handle(std::string& assign_synonym,
                                                Declaration& lhs,
                                                Wildcard& rhs,
                                                ReadFacade& pkb_reader) {
  IntStringPairSet results = pkb_reader.getAssignPair(rhs);
  return {{assign_synonym, lhs.synonym}, EvaluatorUtil::ToStringPairSet(results)};
}
UnaryConstraint AssignPatternEvaluator::Handle(std::string& assign_synonym,
                                               Wildcard& lhs,
                                               std::string& partial_match,
                                               ReadFacade& pkb_reader) {
  std::unordered_set<int> results = pkb_reader.getAssigns(lhs, partial_match);
  return {assign_synonym, EvaluatorUtil::ToStringSet(results)};
}
UnaryConstraint AssignPatternEvaluator::Handle(std::string& assign_synonym,
                                               Wildcard& lhs,
                                               Wildcard& rhs,
                                               ReadFacade& pkb_reader) {
  std::unordered_set<int> results = pkb_reader.getAssigns(lhs, rhs);
  return {assign_synonym, EvaluatorUtil::ToStringSet(results)};
}
UnaryConstraint AssignPatternEvaluator::Handle(std::string& assign_synonym,
                                               std::string& lhs,
                                               std::string& partial_match,
                                               ReadFacade& pkb_reader) {
  std::unordered_set<int> results = pkb_reader.getAssigns(lhs, partial_match);
  return {assign_synonym, EvaluatorUtil::ToStringSet(results)};
}
UnaryConstraint AssignPatternEvaluator::Handle(std::string& assign_synonym,
                                               std::string& lhs,
                                               Wildcard& rhs,
                                               ReadFacade& pkb_reader) {
  std::unordered_set<int> results = pkb_reader.getAssigns(lhs, rhs);
  return {assign_synonym, EvaluatorUtil::ToStringSet(results)};
}
