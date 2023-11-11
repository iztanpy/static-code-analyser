//
// Created by Isaac Tan on 5/10/23.
//

#include "IfStore.h"
typedef std::string variable;
typedef int statementNumber;

IfStore::IfStore() {
  variableMap = std::unordered_map<statementNumber, std::unordered_set<variable>>();
  reverseVariableMap = std::unordered_map<variable, std::unordered_set<statementNumber>>();
}

void IfStore::addVariableMap(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap) {
  this->variableMap = variableMap;
  for (auto const& [key, value] : variableMap) {
    for (auto const& x : value) {
      reverseVariableMap[x].insert(key);
    }
  }
}

std::unordered_set<statementNumber> IfStore::getIf(Wildcard wc) {
  std::unordered_set<statementNumber> result;
  for (auto const& [key, value] : variableMap) {
    result.insert(key);
  }
  return result;
}

std::unordered_set<statementNumber> IfStore::getIf(variable v) {
  std::unordered_set<statementNumber> result;
  if (reverseVariableMap.find(v) != reverseVariableMap.end()) {
    for (auto const& x : reverseVariableMap[v]) {
      result.insert(x);
    }
  }
  return result;
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> IfStore::getAllIf() {
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> result;
  for (auto const& [key, value] : variableMap) {
    for (auto const& x : value) {
      result.insert(std::make_pair(key, x));
    }
  }
  return result;
}
