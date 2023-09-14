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
    std::unordered_map<statementNumber, std::unordered_set<constant>> UsesConstantMap;


 public:
    UsesStore();

    void addLineUsesVar(std::unordered_map<statementNumber, std::unordered_set<variable>> statementUsesMap);

    void addLineUsesConst(std::unordered_map<statementNumber, std::unordered_set<constant>> variableUsesMap);

    std::unordered_set<variable> getVariablesUsedBy(statementNumber lineNumber);

    std::unordered_set<statementNumber> getStatementsUsing(variable variableName);

    std::unordered_set<constant> getConstantsUsedBy(statementNumber lineNumber);
};
