#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include "PKB/PKB.h"

typedef int statementNumber;
typedef std::string variable;
typedef std::string constant;
typedef std::string possibleCombinations;


class ReadFacade {
 private:
  PKB* pkb;
 public:
  explicit ReadFacade(PKB* pkbptr);

  std::unordered_set<statementNumber> getAllAssigns();
  std::unordered_set<variable> getAllVariables();
  std::unordered_set<constant> getAllConstants();
  std::unordered_set<variable> getVariablesUsedBy(int lineNumber);

  // unimplemented
  std::unordered_set<statementNumber> getLinesthatUse(variable variableName);
  std::unordered_set<statementNumber> getAssign(variable LHS, possibleCombinations RHS);
};
