#include <stdio.h>
#include <iostream>
#include <unordered_set>

#include "PKB/API/ReadFacade.h"

ReadFacade::ReadFacade(PKB* pkbptr) : pkb(pkbptr) {}

std::unordered_set<int> ReadFacade::getAllAssigns() {
    return {1, 2, 3, 4, 5, 6, 7, 8, 9};
}

std::unordered_set<std::string> ReadFacade::getAllVariables() {
    return {"x", "y", "z", "a", "b", "c", "d", "e", "f", "g", "h"};
}

std::unordered_set<std::string> ReadFacade::getAllConstants() {
    return {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
}

std::unordered_set<std::string> ReadFacade::getVariablesUsedBy(int lineNumber) {
    return {"x", "y", "z"};
}
