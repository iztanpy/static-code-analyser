#include "ModifiesStore.h"
#include <unordered_map>
#include <utility>
#include <iostream>

typedef std::string variable;
typedef std::string constant;
typedef int statementNumber;
typedef std::string procedure;

ModifiesStore::ModifiesStore() {
    std::unordered_map<statementNumber, std::unordered_set<variable>> ModifiesVariableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> ModifiesVariableMapReverse;
    std::unordered_map<procedure, std::unordered_set<variable>> ModifiesProcedureMap;
    std::unordered_map<variable, std::unordered_set<procedure>> ModifiesProcedureMapReverse;
}

void ModifiesStore::storeModifies(std::unordered_map<statementNumber, std::unordered_set<variable>> relations) {
    this->ModifiesVariableMap = relations;
    for (auto const& x : relations) {
        for (auto const& y : x.second) {
            this->ModifiesVariableMapReverse[y].insert(x.first);
        }
    }
}

void ModifiesStore::storeModifiesProcedures(std::unordered_map<procedure,
    std::pair<int, int>> procedures,
    std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar) {
    for (auto const& x : procedures) {
        std::unordered_set<variable> variables;
        for (int i = x.second.first; i <= x.second.second; i++) {
            if (ModifiesVariableMap.find(i) != ModifiesVariableMap.end()) {
                for (auto const& y : ModifiesVariableMap[i]) {
                    variables.insert(y);
                }
            }
        }
        ModifiesProcedureMap[x.first] = variables;
        for (auto const& y : variables) {
            ModifiesProcedureMapReverse[y].insert(x.first);
        }
    }
    for (auto const& x : callTableStar) {
        for (auto const& y : x.second) {
            if (ModifiesProcedureMap.find(y) != ModifiesProcedureMap.end()) {
                for (auto const& z : ModifiesProcedureMap[y]) {
                    ModifiesProcedureMap[x.first].insert(z);
                    ModifiesProcedureMapReverse[z].insert(x.first);
                }
            }
        }
    }
}

void ModifiesStore::storeModifiesCalls(std::unordered_map<statementNumber, procedure> calls) {
    for (auto const& x : calls) {
        ModifiesVariableMap[x.first].insert(ModifiesProcedureMap[x.second].begin(), ModifiesProcedureMap[x.second].end());
        for (auto const& y : ModifiesProcedureMap[x.second]) {
            ModifiesVariableMapReverse[y].insert(x.first);
        }
    }
}

bool ModifiesStore::isModifies(statementNumber statement, variable variable) {
    if (ModifiesVariableMap.find(statement) != ModifiesVariableMap.end()) {
        if (ModifiesVariableMap[statement].find(variable) != ModifiesVariableMap[statement].end()) {
            return true;
        }
    }
    return false;
}

bool ModifiesStore::isModifies(statementNumber statement) {
    if (ModifiesVariableMap.find(statement) != ModifiesVariableMap.end()) {
        return true;
    }
    return false;
}

std::unordered_set<statementNumber> ModifiesStore::modifies(ModifiesStore::variable variable) {
    std::unordered_set<statementNumber> statements = ModifiesVariableMapReverse[variable];
    return statements;
}

std::unordered_set<variable> ModifiesStore::modifies(ModifiesStore::statementNumber statement) {
    return ModifiesVariableMap[statement];
}

bool ModifiesStore::isModifies(procedure procedure) {
    if (ModifiesProcedureMap.find(procedure) != ModifiesProcedureMap.end()) {
        return true;
    }
    return false;
}

std::unordered_set<variable> ModifiesStore::modifiesProcedureProc(procedure procedure) {
    return ModifiesProcedureMap[procedure];
}

bool ModifiesStore::isModifies(procedure procedure, variable variable) {
    if (ModifiesProcedureMap.find(procedure) != ModifiesProcedureMap.end()) {
        if (ModifiesProcedureMap[procedure].find(variable) != ModifiesProcedureMap[procedure].end()) {
            return true;
        }
    }
    return false;
}

std::unordered_set<procedure> ModifiesStore::modifiesProcedure() {
    std::unordered_set<procedure> procedures;
    for (auto const& x : ModifiesProcedureMap) {
        procedures.insert(x.first);
    }
    return procedures;
}

std::unordered_set<procedure> ModifiesStore::modifiesProcedure(variable variable) {
    return ModifiesProcedureMapReverse[variable];
}

std::unordered_set<std::pair<procedure, variable>, PairHash> ModifiesStore::modifiesProcedurePair() {
    std::unordered_set<std::pair<procedure, variable>, PairHash> pairs;
    for (auto const& x : ModifiesProcedureMap) {
        for (auto const& y : x.second) {
            pairs.insert(std::make_pair(x.first, y));
        }
    }
    return pairs;
}
