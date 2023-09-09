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

     std::vector<int> getAllAssigns();
     std::vector<std::string> getAllVariables();
     std::vector<std::string> getAllConstants();
     std::unordered_set<std::string> getVariablesUsedBy(int lineNumber);
};
