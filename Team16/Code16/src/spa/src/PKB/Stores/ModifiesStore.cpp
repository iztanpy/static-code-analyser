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

bool ModifiesStore::isModifies(statementNumber statement, variable variable) {
    if (ModifiesVariableMap.find(statement) != ModifiesVariableMap.end()) {
        if (ModifiesVariableMap[statement] == variable) {
			return true;
		}
	}
	return false;
}

bool ModifiesStore::isModifies(statementNumber statement) {
    if (ModifiesVariableMap.find(statement) != ModifiesVariableMap.end()) {
        if (ModifiesVariableMap[statement].size() > 0) {
			return true;
		}
	}
	return false;
}

std::unordered_set<statementNumber> ModifiesStore::modifies(ModifiesStore::variable variable) {
    std::unordered_set<statementNumber> statements = ModifiesVariableMapReverse[variable];
    return statements;
}

variable ModifiesStore::modifies(ModifiesStore::statementNumber statement) {
    variable variable = ModifiesVariableMap[statement];
    return variable;
}
