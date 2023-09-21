#include "UsesStore.h"
#include <unordered_map>

typedef std::string variable;
typedef std::string constant;
typedef int statementNumber;

UsesStore::UsesStore() {
    std::unordered_map<statementNumber, std::unordered_set<variable>> UsesVariableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> UsesVariableMapReverse;
}

void UsesStore::storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap) {
    this->UsesVariableMap = varUsesMap;
    for (auto const& x : varUsesMap) {
        for (auto const& y : x.second) {
            this->UsesVariableMapReverse[y].insert(x.first);
        }
    }
}

bool UsesStore::isUses(statementNumber lineNumber, variable variableName) {
    if (UsesVariableMap.find(lineNumber) != UsesVariableMap.end()) {
        if (UsesVariableMap[lineNumber].find(variableName) != UsesVariableMap[lineNumber].end()) {
            return true;
        }
    }
    return false;
}

bool UsesStore::isUses(statementNumber lineNumber) {
    if (UsesVariableMap.find(lineNumber) != UsesVariableMap.end()) {
        if (UsesVariableMap[lineNumber].size() > 0) {
            return true;
        }
    }
    return false;
}

std::unordered_set<variable> UsesStore::uses(statementNumber lineNumber) {
    std::unordered_set<variable> variablesUsed = UsesVariableMap[lineNumber];
    return variablesUsed;
}

std::unordered_set<statementNumber> UsesStore::uses(variable variableName) {
    std::unordered_set<statementNumber> statementsUsing = UsesVariableMapReverse[variableName];
    return statementsUsing;
}

