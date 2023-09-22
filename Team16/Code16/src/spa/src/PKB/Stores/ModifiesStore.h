#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>

class ModifiesStore {
 private:
    typedef std::string variable;
    typedef int statementNumber;
    std::unordered_map<statementNumber, std::unordered_set<variable>> ModifiesVariableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> ModifiesVariableMapReverse;

 public:
    ModifiesStore();

    void storeModifies(std::unordered_map<statementNumber, std::unordered_set<variable>> relations);

    bool isModifies(statementNumber statement, variable variable);

    bool isModifies(statementNumber statement);

    std::unordered_set<variable> modifies(statementNumber statement);

    std::unordered_set<statementNumber> modifies(variable variable);
};
