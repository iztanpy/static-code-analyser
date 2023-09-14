#include "UsesStore.h"
#include <unordered_map>

typedef std::string variable;
typedef std::string constant;
typedef int statementNumber;

UsesStore::UsesStore() {
    std::unordered_map<statementNumber, std::unordered_set<variable>> UsesVariableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> UsesVariableMapReverse;
    std::unordered_map<statementNumber, std::unordered_set<constant>> UsesConstantMap;
}

void UsesStore::addLineUsesVar(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap) {
    this->UsesVariableMap = varUsesMap;
}

void UsesStore::addLineUsesConst(std::unordered_map<statementNumber, std::unordered_set<constant>> constantUsesMap) {
    this->UsesConstantMap = constantUsesMap;
}

std::unordered_set<variable> UsesStore::getVariablesUsedBy(statementNumber lineNumber) {
    std::unordered_set<variable> variablesUsed = UsesVariableMap[lineNumber];
    return variablesUsed;
}

std::unordered_set<statementNumber> UsesStore::getStatementsUsing(variable variableName) {
    std::unordered_set<statementNumber> statementsUsing = UsesVariableMapReverse[variableName];
    return statementsUsing;
}

std::unordered_set<constant> UsesStore::getConstantsUsedBy(UsesStore::statementNumber lineNumber) {
    std::unordered_set<constant> constantsUsed = UsesConstantMap[lineNumber];
    return constantsUsed;
}

