#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "PKB/PKB.h"

class WriteFacade {
 private:
     PKB* pkb;

 public:
     explicit WriteFacade(PKB* pkbptr);

     void storeAssignments(std::vector<int> assignments);
     void storeVariables(std::vector<std::string> variables);
     void storeConstants(std::vector<std::string> constants);
};
