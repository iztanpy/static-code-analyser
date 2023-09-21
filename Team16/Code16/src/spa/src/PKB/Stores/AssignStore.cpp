#include "AssignStore.h"
#include <unordered_map>
#include <unordered_set>

typedef std::string variable;
typedef std::string possibleCombinations;
typedef int statementNumber;

AssignStore::AssignStore() {
    this->numLHSMap = std::unordered_map<statementNumber, variable>();
    this->numRHSMap = std::unordered_map<statementNumber, std::unordered_set<partialMatch>>();
    this->reverseNumLHSMap = std::unordered_map<variable, std::unordered_set<statementNumber>> ();
    this->reverseNumRHSMap = std::unordered_map<partialMatch, std::unordered_set<statementNumber>>();
}

void AssignStore::addNumRHSMap(std::unordered_map<statementNumber,
                               std::unordered_set<possibleCombinations>> numRHSMap) {
    this->numRHSMap = numRHSMap;
    for (auto const& x : numRHSMap) {
        for (auto const& y : x.second) {
            reverseNumRHSMap[y].insert(x.first);
        }
    }
}

void AssignStore::addNumLHSMap(std::unordered_map<statementNumber, variable> numLHSMap) {
    this->numLHSMap = numLHSMap;
    for (auto const& x : numLHSMap) {
        reverseNumLHSMap[x.second].insert(x.first);
    }
}


// get all Assign statements
std::unordered_set<statementNumber> AssignStore::getAllAssigns() {
    std::unordered_set<statementNumber> assigns;
    for (auto const& x : this->numLHSMap) {
        assigns.insert(x.first);
    }
    return assigns;
}

std::vector<std::pair<statementNumber, variable>> AssignStore::getAssignPair(partialMatch partial) {
    auto results = std::vector<std::pair<statementNumber, variable>>();
    std::unordered_set<statementNumber> relevantStmt = reverseNumRHSMap[partial];
    for (auto const& x : relevantStmt) {
        std::pair<statementNumber, variable> pair = std::make_pair(x, numLHSMap[x]);
        results.push_back(pair);
    }
    return results;
};

std::vector<std::pair<statementNumber, variable>> AssignStore::getAssignPair(Wildcard wildcard) {
    auto results = std::vector<std::pair<statementNumber, variable>>();
    std::unordered_set<statementNumber> relevantStmt = getAllAssigns();
    for (auto const& x : relevantStmt) {
        std::pair<statementNumber, variable> pair = std::make_pair(x, numLHSMap[x]);
        results.push_back(pair);
    }
    return results;

};

std::unordered_set<statementNumber> AssignStore::getAssigns(Wildcard lhs, partialMatch rhs) {
    return reverseNumRHSMap[rhs];
};

std::unordered_set<statementNumber> AssignStore::getAssigns(Wildcard lhs, Wildcard rhs) {
    return getAllAssigns();
};

std::unordered_set<statementNumber> AssignStore::getAssigns(partialMatch lhs, partialMatch rhs) {
    std::unordered_set<statementNumber> results;
    std::unordered_set<statementNumber> relevantStmt = reverseNumRHSMap[rhs];
    for (auto const& x : relevantStmt) {
        if (numLHSMap[x] == lhs) {
            results.insert(x);
        }
    }
    return results;
};

std::unordered_set<statementNumber> AssignStore::getAssigns(partialMatch lhs, Wildcard rhs) {
    return reverseNumLHSMap[lhs];
};