#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>

#include "PKB.h"


PKB::PKB(): assignStore() {
}

// AssignStore
void PKB::setAssignments(std::unordered_map<statementNumber,
	std::unordered_set<possibleCombinations>> numRHSMap, std::unordered_map<statementNumber, variable> numLHSMap) {
	assignStore.addNumLHSMap(numLHSMap);
	assignStore.addNumRHSMap(numRHSMap);
}

std::unordered_set<statementNumber> PKB::getAllAssigns() {
	return assignStore.getAllAssigns();
}

std::unordered_set<statementNumber> PKB::getAssigns(variable LHS, possibleCombinations RHS) {
	return assignStore.getAssigns(LHS, RHS);
}



