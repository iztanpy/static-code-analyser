#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>


class UsesStore {
 private:
    typedef std::string variable;
    typedef std::string constant;
    typedef int statementNumber;

    std::unordered_map<statementNumber, std::unordered_set<variable>> UsesVariableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> UsesVariableMapReverse;


 public:
    UsesStore();

    void storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> statementUsesMap);

    std::unordered_set<variable> getVariablesUsedBy(statementNumber lineNumber);

    std::unordered_set<statementNumber> getStatementsUsing(variable variableName);
};
