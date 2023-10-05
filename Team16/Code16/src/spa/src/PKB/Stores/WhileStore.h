#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "utils/clauses_types.h"
#include "utils/entity_types.h"
#include "utils/hash_utils.h"

class WhileStore {
private:
    typedef std::string variable;
    typedef int statementNumber;
    std::unordered_map<statementNumber, variable> variableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> reverseVariableMap;

public:
    WhileStore();

    void addVariableMap(std::unordered_map<statementNumber, variable> variableMap);
};