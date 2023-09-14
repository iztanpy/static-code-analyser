#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include "pkb/PKB.h"

class ReadFacade {
 private:
  PKB& pkb;
 public:
  explicit ReadFacade(PKB& pkb);

  std::unordered_set<statementNumber> getAllAssigns();

  std::unordered_set<statementNumber> getAssigns(variable LHS, possibleCombinations RHS);

  std::unordered_set<variable> getAllVariables();

  std::unordered_set<constant> getAllConstants();

  std::unordered_set<variable> getVariablesUsedBy(statementNumber line);

  std::unordered_set<constant> getConstantsUsedBy(statementNumber line);
};
