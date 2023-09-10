#include <iostream>
#include <vector>
#include <unordered_set>

#include "pkb/pkb.h"
#include "pkb/api/read_facade.h"

ReadFacade::ReadFacade(PKB* pkbptr) : pkb(pkbptr) {}

std::unordered_set<int> ReadFacade::GetAllAssigns() {
  return {1, 2, 3};
//  return pkb->getAssignments();
}

std::unordered_set<std::string> ReadFacade::GetAllVariables() {
  return {"bar", "foo"};
//  return pkb->getVariables();
}

std::unordered_set<std::string> ReadFacade::GetAllConstants() {
  return {"kBar", "kFoo"};
//  return pkb->getConstants();
}

std::unordered_set<std::string> ReadFacade::GetVariablesUsedBy(int line_number) {
  return {"bar", "foo"};
//  return pkb->getVariablesUsedBy(lineNumber);
}
