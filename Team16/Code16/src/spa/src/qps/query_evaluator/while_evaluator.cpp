#include "qps/query_evaluator/while_evaluator.h"

BinaryConstraint WhileEvaluator::Handle(std::string& synonym, Declaration& lhs, ReadFacade& pkb_reader) {
  IntStringPairSet results = pkb_reader.getAllWhile();
  return {{synonym, lhs.synonym}, EvaluatorUtil::ToStringPairSet(results)};
}

UnaryConstraint WhileEvaluator::Handle(std::string& synonym, Wildcard& lhs, ReadFacade& pkb_reader) {
  std::unordered_set<int> results = pkb_reader.getWhile(lhs);
  return {synonym, EvaluatorUtil::ToStringSet(results)};
}

UnaryConstraint WhileEvaluator::Handle(std::string& synonym, std::string& lhs, ReadFacade& pkb_reader) {
  std::unordered_set<int> results = pkb_reader.getWhile(lhs);
  return {synonym, EvaluatorUtil::ToStringSet(results)};
}
