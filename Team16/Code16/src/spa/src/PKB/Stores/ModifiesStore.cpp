#include "ModifiesStore.h"
#include <unordered_map>

typedef std::string variable;
typedef std::string constant;
typedef int statementNumber;

ModifiesStore::ModifiesStore() {
    std::unordered_map<statementNumber, variable> ModifiesVariableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> ModifiesVariableMapReverse;
}

void ModifiesStore::storeModifies(std::unordered_map<statementNumber, variable> relations) {
    this->ModifiesVariableMap = relations;
    for (auto const& x : relations) {
        this->ModifiesVariableMapReverse[x.second].insert(x.first);
    }
}

std::unordered_set<statementNumber> ModifiesStore::getStatementThatModifies(ModifiesStore::variable variable) {
    std::unordered_set<statementNumber> statements = ModifiesVariableMapReverse[variable];
    return statements;
}

variable ModifiesStore::getVariableModifiedBy(ModifiesStore::statementNumber statement) {
    variable variable = ModifiesVariableMap[statement];
    return variable;
}