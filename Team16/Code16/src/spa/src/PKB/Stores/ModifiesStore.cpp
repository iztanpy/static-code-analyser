#include "ModifiesStore.h"
#include <unordered_map>

typedef std::string variable;
typedef std::string constant;
typedef int statementNumber;

ModifiesStore::ModifiesStore() {
    std::unordered_map<statementNumber, std::unordered_set<variable>> ModifiesVariableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> ModifiesVariableMapReverse;
}

void ModifiesStore::storeModifies(std::unordered_map<statementNumber, std::unordered_set<variable>> relations) {
    this->ModifiesVariableMap = relations;
    for (auto const& x : relations) {
        for (auto const& y : x.second) {
            this->ModifiesVariableMapReverse[y].insert(x.first);
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
