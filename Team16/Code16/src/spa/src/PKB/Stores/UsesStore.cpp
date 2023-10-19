#include "UsesStore.h"
#include <utils/hash_utils.h>
#include <unordered_map>

typedef std::string variable;
typedef std::string constant;
typedef int statementNumber;
typedef std::string procedure;

UsesStore::UsesStore() {
    std::unordered_map<statementNumber, std::unordered_set<variable>> UsesVariableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> UsesVariableMapReverse;
    std::unordered_map<procedure, std::unordered_set<variable>> UsesProcedureMap;
    std::unordered_map<variable, std::unordered_set<procedure>> UsesProcedureMapReverse;
}

void UsesStore::storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap) {
    this->UsesVariableMap = varUsesMap;
    for (auto const& x : varUsesMap) {
        for (auto const& y : x.second) {
            this->UsesVariableMapReverse[y].insert(x.first);
        }
    }
}

void UsesStore::storeUsesProcedures(std::unordered_map<procedure,
    std::pair<int, int>> procedures,
    std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar) {
    for (auto const& x : procedures) {
        std::unordered_set<variable> variables;
        for (int i = x.second.first; i <= x.second.second; i++) {
            if (UsesVariableMap.find(i) != UsesVariableMap.end()) {
                for (auto const& y : UsesVariableMap[i]) {
                    variables.insert(y);
                }
            }
        }
        UsesProcedureMap[x.first] = variables;
        for (auto const& y : variables) {
            UsesProcedureMapReverse[y].insert(x.first);
        }
    }
    for (auto const& x : callTableStar) {
        for (auto const& y : x.second) {
            if (UsesProcedureMap.find(y) != UsesProcedureMap.end()) {
                for (auto const& z : UsesProcedureMap[y]) {
                    UsesProcedureMap[x.first].insert(z);
                    UsesProcedureMapReverse[z].insert(x.first);
                }
            }
        }
    }
}

void UsesStore::storeUsesCalls(std::unordered_map<statementNumber, procedure> calls) {
    for (auto const& x : calls) {
        UsesVariableMap[x.first].insert(UsesProcedureMap[x.second].begin(), UsesProcedureMap[x.second].end());
        for (auto const& y : UsesProcedureMap[x.second]) {
            UsesVariableMapReverse[y].insert(x.first);
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

bool UsesStore::isUses(procedure procedure) {
    if (UsesProcedureMap.find(procedure) != UsesProcedureMap.end()) {
        if (UsesProcedureMap[procedure].size() > 0) {
            return true;
        }
    }
    return false;
}

std::unordered_set<variable> UsesStore::usesProcedureProc(procedure procedure) {
    return UsesProcedureMap[procedure];
}

bool UsesStore::isUses(procedure procedure, variable variable) {
    if (UsesProcedureMap.find(procedure) != UsesProcedureMap.end()) {
        if (UsesProcedureMap[procedure].find(variable) != UsesProcedureMap[procedure].end()) {
            return true;
        }
    }
    return false;
}

std::unordered_set<procedure> UsesStore::usesProcedure() {
    std::unordered_set<procedure> procedures;
    for (auto const& x : UsesProcedureMap) {
        if (x.second.size() > 0) {
            procedures.insert(x.first);
        }
    }
    return procedures;
}

std::unordered_set<procedure> UsesStore::usesProcedure(variable variable) {
    return UsesProcedureMapReverse[variable];
}

std::unordered_set<std::pair<procedure, variable>, PairHash> UsesStore::usesProcedurePair() {
    std::unordered_set<std::pair<procedure, variable>, PairHash> pairs;
    for (auto const& x : UsesProcedureMap) {
        for (auto const& y : x.second) {
            pairs.insert(std::make_pair(x.first, y));
        }
    }
    return pairs;
}
