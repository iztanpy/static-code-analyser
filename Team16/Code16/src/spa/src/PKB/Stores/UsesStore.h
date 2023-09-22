#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "utils/clauses_types.h"



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

    bool isUses(statementNumber lineNumber, variable variableName);

    bool isUses(statementNumber lineNumber);

    std::unordered_set<variable> uses(statementNumber lineNumber);

    std::unordered_set<statementNumber> uses(variable variableName);
};
