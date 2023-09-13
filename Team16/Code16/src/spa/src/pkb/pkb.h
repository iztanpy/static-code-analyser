#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Stores/VariableStore.h"
#include "Stores/AssignStore.h"
#include "Stores/ConstantStore.h"
#include "Stores/UsesStore.h"
#include "Stores/StatementStore.h"
#include "Stores/ConstantStore.cpp"
#include "Stores/AssignStore.cpp"




class PKB {
 private:
     AssignStore assignStore;

 public:
     PKB();

     void setAssignments(std::unordered_map<statementNumber,
         std::unordered_set<possibleCombinations>> numRHSMap, std::unordered_map<statementNumber, variable> numLHSMap);

     std::unordered_set<statementNumber> getAllAssigns();

     std::unordered_set<statementNumber> getAssigns(variable LHS, possibleCombinations RHS);


};
