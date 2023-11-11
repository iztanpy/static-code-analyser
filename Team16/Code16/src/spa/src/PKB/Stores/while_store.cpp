//
// Created by Isaac Tan on 5/10/23.
//

#include "PKB/Stores/while_store.h"
typedef int statementNumber;
typedef std::string variable;

while_store::while_store() {
  variableMap = std::unordered_map<statementNumber, std::unordered_set<variable>>();
  reverseVariableMap = std::unordered_map<variable, std::unordered_set<statementNumber>>();
}

void while_store::addVariableMap(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap) {
  this->variableMap = variableMap;
  for (auto const& [key, value] : variableMap) {
    for (auto const& x : value) {
      reverseVariableMap[x].insert(key);
    }
  }
}

std::unordered_set<statementNumber> while_store::getWhile(Wildcard wc) {
  std::unordered_set<statementNumber> result;
  for (auto const& [key, value] : variableMap) {
    result.insert(key);
  }
  return result;
}

std::unordered_set<statementNumber> while_store::getWhile(variable v) {
  std::unordered_set<statementNumber> result;
  if (reverseVariableMap.find(v) != reverseVariableMap.end()) {
    for (auto const& x : reverseVariableMap[v]) {
      result.insert(x);
    }
  }
  return result;
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> while_store::getAllWhile() {
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> result;
  for (auto const& [key, value] : variableMap) {
    for (auto const& x : value) {
      result.insert(std::make_pair(key, x));
    }
  }
  return result;
}
