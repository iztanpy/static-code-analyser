#include "AssignStore.h"

typedef std::string variable;
typedef std::string possibleCombinations;
typedef int statementNumber;

AssignStore::AssignStore() {
  this->numLHSMap = std::unordered_map<statementNumber, variable>();
  this->numRHSMap = std::unordered_map<statementNumber, std::unordered_set<partialMatch>>();
  this->reverseNumLHSMap = std::unordered_map<variable, std::unordered_set<statementNumber>>();
  this->reverseNumRHSMap = std::unordered_map<partialMatch, std::unordered_set<statementNumber>>();

  this->fullRHSMap = std::unordered_map<statementNumber, std::unordered_set<fullRHS>>();
  this->reverseFullRHSMap = std::unordered_map<fullRHS, std::unordered_set<statementNumber>>();
  this->partialRHSMap = std::unordered_map<statementNumber, std::unordered_set<partialMatch>>();
  this->reversePartialRHSMap = std::unordered_map<partialMatch, std::unordered_set<statementNumber>>();
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

void AssignStore::storeFullPatternAssign(std::unordered_map<statementNumber,
                                         std::unordered_set<fullRHS>> relations) {
    this->fullRHSMap = relations;
    for (auto const& x : relations) {
        for (auto const& y : x.second) {
            reverseFullRHSMap[y].insert(x.first);
        }
    }
}

void AssignStore::storeAllPossibleCombinationsAssign(std::unordered_map<statementNumber,
                                                     std::unordered_set<partialMatch>> relations) {
    this->partialRHSMap = relations;
    for (auto const& x : relations) {
        for (auto const& y : x.second) {
            reversePartialRHSMap[y].insert(x.first);
        }
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

std::unordered_set<std::pair<statementNumber, variable>, PairHash> AssignStore::getAssignPair(partialMatch partial) {
  auto results = std::unordered_set<std::pair<statementNumber, variable>, PairHash>();
  std::unordered_set<statementNumber> relevantStmt = reverseNumRHSMap[partial];
  for (auto const& x : relevantStmt) {
    std::pair<statementNumber, variable> pair = std::make_pair(x, numLHSMap[x]);
    results.insert(pair);
  }
  return results;
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> AssignStore::getAssignPair(Wildcard wildcard) {
  auto results = std::unordered_set<std::pair<statementNumber, variable>, PairHash>();
  std::unordered_set<statementNumber> relevantStmt = getAllAssigns();
  for (auto const& x : relevantStmt) {
    std::pair<statementNumber, variable> pair = std::make_pair(x, numLHSMap[x]);
    results.insert(pair);
  }
  return results;
}

std::unordered_set<statementNumber> AssignStore::getAssigns(Wildcard lhs, partialMatch rhs) {
  return reverseNumRHSMap[rhs];
}

std::unordered_set<statementNumber> AssignStore::getAssigns(Wildcard lhs, Wildcard rhs) {
  return getAllAssigns();
}

std::unordered_set<statementNumber> AssignStore::getAssigns(partialMatch lhs, partialMatch rhs) {
  std::unordered_set<statementNumber> results;
  std::unordered_set<statementNumber> relevantStmt = reverseNumRHSMap[rhs];
  for (auto const& x : relevantStmt) {
    if (numLHSMap[x] == lhs) {
      results.insert(x);
    }
  }
  return results;
}

std::unordered_set<statementNumber> AssignStore::getAssigns(partialMatch lhs, Wildcard rhs) {
  return reverseNumLHSMap[lhs];
}
