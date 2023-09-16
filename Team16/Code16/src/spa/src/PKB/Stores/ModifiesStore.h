#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>

class ModifiesStore {
 private:
    typedef std::string variable;
    typedef int statementNumber;
    std::unordered_map<statementNumber, variable> ModifiesVariableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> ModifiesVariableMapReverse;

 public:
    ModifiesStore();

    void storeModifies(std::unordered_map<statementNumber, variable> relations);
    std::unordered_set<statementNumber> getStatementThatModifies(variable variable);
    variable getVariableModifiedBy(statementNumber statement);
};
