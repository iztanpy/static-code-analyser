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

  std::unordered_set<int> GetAllAssigns();
  std::unordered_set<std::string> GetAllVariables();
  std::unordered_set<std::string> GetAllConstants();
  std::unordered_set<std::string> GetVariablesUsedBy(int line_numer);
};
