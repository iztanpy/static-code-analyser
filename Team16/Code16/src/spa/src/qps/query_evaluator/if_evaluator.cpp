#include "qps/query_evaluator/if_evaluator.h"

BinaryConstraint IfEvaluator::Handle(std::string& synonym, Declaration& lhs, ReadFacade& pkb_reader) {
  IntStringPairSet results = pkb_reader.getAllIf();
  return {{synonym, lhs.synonym}, EvaluatorUtil::ToStringPairSet(results)};
}

UnaryConstraint IfEvaluator::Handle(std::string& synonym, Wildcard& lhs, ReadFacade& pkb_reader) {
  std::unordered_set<int> results = pkb_reader.getIf(lhs);
  return {synonym, EvaluatorUtil::ToStringSet(results)};
}

UnaryConstraint IfEvaluator::Handle(std::string& synonym, std::string& lhs, ReadFacade& pkb_reader) {
  std::unordered_set<int> results = pkb_reader.getIf(lhs);
  return {synonym, EvaluatorUtil::ToStringSet(results)};
}
