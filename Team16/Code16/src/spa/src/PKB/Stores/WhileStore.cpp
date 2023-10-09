//
// Created by Isaac Tan on 5/10/23.
//

#include "WhileStore.h"
typedef int statementNumber;
typedef std::string variable;


WhileStore::WhileStore() {
    variableMap = std::unordered_map<statementNumber, variable>();
    reverseVariableMap = std::unordered_map<variable, std::unordered_set<statementNumber>>();
}

void WhileStore::addVariableMap(std::unordered_map<statementNumber, variable> variableMap) {
    this->variableMap = variableMap;
    for (auto const& [key, value] : variableMap) {
        reverseVariableMap[value].insert(key);
    }
}

std::unordered_set<statementNumber> WhileStore::getWhile(Wildcard wc) {
    std::unordered_set<statementNumber> result;
    for (auto const& [key, value] : variableMap) {
        result.insert(key);
    }
    return result;
}

std::unordered_set<statementNumber> WhileStore::getWhile(variable v) {
    std::unordered_set<statementNumber> result;
    if (reverseVariableMap.find(v) != reverseVariableMap.end()) {
        for (auto const& x : reverseVariableMap[v]) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> WhileStore::getAllWhile() {
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> result;
    for (auto const& [key, value] : variableMap) {
        result.insert(std::make_pair(key, value));
    }
    return result;
}

