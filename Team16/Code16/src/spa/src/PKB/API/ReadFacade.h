#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include "PKB/PKB.h"

class ReadFacade {
 private:
     PKB* pkb;
 public:
     explicit ReadFacade(PKB* pkbptr);

     std::unordered_set<int> getAllAssigns();
     std::unordered_set<std::string> getAllVariables();
     std::unordered_set<std::string> getAllConstants();
     std::unordered_set<std::string> getVariablesUsedBy(int lineNumber);
};
