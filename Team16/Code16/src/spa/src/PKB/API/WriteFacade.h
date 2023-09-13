#pragma once
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "PKB/PKB.h"

class WriteFacade {
 private:
    PKB& pkb;

 public:
    explicit WriteFacade(PKB& pkb);

    void storeAssignments(std::unordered_set<int> assignments);

    void storeVariables(std::unordered_set<std::string> variables);
    void storeConstants(std::unordered_set<std::string> constants);

    void storeUsesConst(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS);
    void storeUsesVar(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS);

    void storeLineUses(std::unordered_map<int, std::unordered_set<std::string>> usesVariableAndConstants);
};
