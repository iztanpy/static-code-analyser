//
// Created by Isaac Tan on 5/10/23.
//

#include "PKB/Stores/if_store.h"
typedef std::string variable;
typedef int statementNumber;

if_store::if_store() {
  variableMap = std::unordered_map<statementNumber, std::unordered_set<variable>>();
  reverseVariableMap = std::unordered_map<variable, std::unordered_set<statementNumber>>();
}

void if_store::addVariableMap(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap) {
  this->variableMap = variableMap;
  for (auto const& [key, value] : variableMap) {
    for (auto const& x : value) {
      reverseVariableMap[x].insert(key);
    }
  }
}

std::unordered_set<statementNumber> if_store::getIf(Wildcard wc) {
  std::unordered_set<statementNumber> result;
  for (auto const& [key, value] : variableMap) {
    result.insert(key);
  }
  return result;
}

std::unordered_set<statementNumber> if_store::getIf(variable v) {
  std::unordered_set<statementNumber> result;
  if (reverseVariableMap.find(v) != reverseVariableMap.end()) {
    for (auto const& x : reverseVariableMap[v]) {
      result.insert(x);
    }
  }
  return result;
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> if_store::getAllIf() {
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> result;
  for (auto const& [key, value] : variableMap) {
    for (auto const& x : value) {
      result.insert(std::make_pair(key, x));
    }
  }
  return result;
}
