//
// Created by Isaac Tan on 13/9/23.
//

#include "AssignStore.h"
typedef std::string variable;
typedef std::string possibleCombinations;
typedef int statementNumber;

AssignStore::AssignStore() {
    this->numLHSMap = std::unordered_map<statementNumber, variable>();
    this->numRHSMap = std::unordered_map<statementNumber, std::unordered_set<possibleCombinations>>();
}

void AssignStore::addNumRHSMap(std::unordered_map<statementNumber, std::unordered_set<possibleCombinations>> numRHSMap) {
    this->numRHSMap = numRHSMap;
}

void AssignStore::addNumLHSMap(std::unordered_map<statementNumber, variable> numLHSMap) {
    this->numLHSMap = numLHSMap;
}


// get all Assign statements
std::unordered_set<statementNumber> AssignStore::getAllAssigns() {
    std::unordered_set<statementNumber> assigns;
    for (auto const& x : this->numLHSMap) {
        assigns.insert(x.first);
    }
    return assigns;
}

// get Assign statements that fulfil requirements
std::unordered_set<statementNumber> AssignStore::getAssigns(AssignStore::variable LHS,
                                                            AssignStore::possibleCombinations RHS) {
    std::unordered_set<statementNumber> assigns;

    if (LHS == "_") {
        // only need to match RHS
        for (auto const& x : this->numRHSMap) {
            for (auto const& y : x.second) {
                if (y == RHS) {
                    assigns.insert(x.first);
                }
            }
            return assigns;
        }
        return assigns;
    } else if (RHS == "_") {
        // only need to match LHS
        for (auto const& x : this->numLHSMap) {
            if (x.second == LHS) {
                assigns.insert(x.first);
            }
        }
        return assigns;
    }
    for (auto const& x : this->numLHSMap) {
        if (x.second == LHS) {
            for (auto const& y : this->numRHSMap[x.first]) {
                if (y == RHS) {
                    assigns.insert(x.first);
                }
            }
        }
    }
    return assigns;
}

