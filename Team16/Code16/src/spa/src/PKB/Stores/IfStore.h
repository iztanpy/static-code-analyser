#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "utils/clauses_types.h"
#include "utils/entity_types.h"
#include "utils/hash_utils.h"


class IfStore {
 private:
    typedef std::string variable;
    typedef int statementNumber;
    std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> reverseVariableMap;

 public:
    IfStore();

    void addVariableMap(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap);
    std::unordered_set<statementNumber> getIf(Wildcard wc);
    std::unordered_set<statementNumber> getIf(variable v);
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAllIf();
};

