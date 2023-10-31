#include "qps/query_evaluator/with_evaluator.h"

namespace {
BinaryConstraint HandleComplexSimple(AttrRef& complex, AttrRef& simple, ReadFacade& pkb_reader) {
  assert(complex.IsComplexCase());

  BinaryConstraint complex_result = std::get<BinaryConstraint>(complex.Evaluate(pkb_reader));
  UnaryConstraint simple_result = std::get<UnaryConstraint>(simple.Evaluate(pkb_reader));
  BinaryConstraint results;
  for (auto& result : complex_result.pair_values) {
    if (simple_result.values.find(result.second) != simple_result.values.end()) {
      results.pair_values.insert(result);
    }
  }
  results.pair_col_names = {complex.declaration.synonym, simple.declaration.synonym};
  return results;
}
}  // anonymous namespace

bool WithEvaluator::Handle(int lhs, int rhs, ReadFacade& pkb_reader) {
  return lhs == rhs;
}

bool WithEvaluator::Handle(std::string& lhs, std::string& rhs, ReadFacade& pkb_reader) {
  return lhs == rhs;
}

bool WithEvaluator::Handle(int integer, std::string& string, ReadFacade& pkb_reader) {
  assert(false && "[WithEvaluator] This code should never be reached! Make sure to call Validate() within constructor");
  return false;
}

bool WithEvaluator::Handle(std::string& string, int integer, ReadFacade& pkb_reader) {
  return WithEvaluator::Handle(integer, string, pkb_reader);
}

UnaryConstraint WithEvaluator::Handle(AttrRef& attr, int integer, ReadFacade& pkb_reader) {
  // Since rhs is an int, lhs cannot be call.procName, read.varName, print.varName
  return UnaryConstraint{attr.declaration.synonym, {std::to_string(integer)}};
}

UnaryConstraint WithEvaluator::Handle(int integer, AttrRef& attr, ReadFacade& pkb_reader) {
  return WithEvaluator::Handle(attr, integer, pkb_reader);
}

UnaryConstraint WithEvaluator::Handle(AttrRef& attr, std::string& string, ReadFacade& pkb_reader) {
  if (attr.IsComplexCase()) {
    BinaryConstraint raw_results = std::get<BinaryConstraint>(attr.Evaluate(pkb_reader));
    std::unordered_set<std::string> results;
    for (auto& result : raw_results.pair_values) {
      if (result.second == string) {
        results.insert(result.first);
      }
    }
    return UnaryConstraint{attr.declaration.synonym, results};
  } else {
    return UnaryConstraint{attr.declaration.synonym, {string}};
  }
}

UnaryConstraint WithEvaluator::Handle(std::string& string, AttrRef& attr, ReadFacade& pkb_reader) {
  return WithEvaluator::Handle(attr, string, pkb_reader);
}

Constraint WithEvaluator::Handle(AttrRef& lhs, AttrRef& rhs, ReadFacade& pkb_reader) {
  if (lhs == rhs) {
    return true;
  }

  if (!lhs.IsComplexCase() && !rhs.IsComplexCase()) {
    UnaryConstraint lhs_results = std::get<UnaryConstraint>(lhs.Evaluate(pkb_reader));
    UnaryConstraint rhs_results = std::get<UnaryConstraint>(rhs.Evaluate(pkb_reader));
    std::unordered_set<Cell> intersection_values = algorithm_utils::intersect(lhs_results.values, rhs_results.values);
    BinaryConstraint results;
    for (auto& value : intersection_values) {
      results.pair_values.insert({value, value});
    }
    results.pair_col_names = {lhs.declaration.synonym, rhs.declaration.synonym};
    return results;
  } else if (lhs.IsComplexCase() && !rhs.IsComplexCase()) {
    return HandleComplexSimple(lhs, rhs, pkb_reader);
  } else if (!lhs.IsComplexCase() && rhs.IsComplexCase()) {
    return HandleComplexSimple(rhs, lhs, pkb_reader);
  } else {
    // Handle both complex case
    BinaryConstraint result;
    result.pair_col_names = {lhs.declaration.synonym, rhs.declaration.synonym};

    std::unordered_map<Cell, Cell> mapLhsToRhs;

    BinaryConstraint lhs_results = std::get<BinaryConstraint>(lhs.Evaluate(pkb_reader));
    BinaryConstraint rhs_results = std::get<BinaryConstraint>(rhs.Evaluate(pkb_reader));

    std::unordered_multimap<std::string, std::string> map;
    for (const auto& p : lhs_results.pair_values) {
      map.insert({p.second, p.first});
    }

    // Iterate over the second set and find matches in the map
    for (const auto& p : rhs_results.pair_values) {
      auto range = map.equal_range(p.second);
      for (auto it = range.first; it != range.second; ++it) {
        result.pair_values.emplace(it->second, p.first);
      }
    }
    return result;
  }
}
