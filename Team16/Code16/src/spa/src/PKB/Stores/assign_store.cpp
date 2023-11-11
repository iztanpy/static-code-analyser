#include "PKB/Stores/assign_store.h"

typedef std::string variable;
typedef std::string possibleCombinations;
typedef int statementNumber;

assign_store::assign_store() {
  this->numLHSMap = std::unordered_map<statementNumber, variable>();
  this->numRHSMap = std::unordered_map<statementNumber, std::unordered_set<partialMatch>>();
  this->reverseNumLHSMap = std::unordered_map<variable, std::unordered_set<statementNumber>>();
  this->reverseNumRHSMap = std::unordered_map<partialMatch, std::unordered_set<statementNumber>>();

  this->fullRHSMap = std::unordered_map<statementNumber, full>();
  this->reverseFullRHSMap = std::unordered_map<full, std::unordered_set<statementNumber>>();
}

void assign_store::addNumRHSMap(std::unordered_map<statementNumber,
                                                  std::unordered_set<possibleCombinations>> numRHSMap) {
  this->numRHSMap = numRHSMap;
  for (auto const& x : numRHSMap) {
    for (auto const& y : x.second) {
      reverseNumRHSMap[y].insert(x.first);
    }
  }
}

void assign_store::addNumLHSMap(std::unordered_map<statementNumber, variable> numLHSMap) {
  this->numLHSMap = numLHSMap;
  for (auto const& x : numLHSMap) {
    reverseNumLHSMap[x.second].insert(x.first);
  }
}

void assign_store::storeFullPatternAssign(std::unordered_map<statementNumber, full> relations) {
  this->fullRHSMap = relations;
  for (auto const& x : relations) {
    reverseFullRHSMap[x.second].insert(x.first);
  }
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash>
assign_store::getAssignPairPartial(partialMatch partial) {
  auto results = std::unordered_set<std::pair<statementNumber, variable>, PairHash>();
  std::unordered_set<statementNumber> relevantStmt = reverseNumRHSMap[partial];
  for (auto const& x : relevantStmt) {
    std::pair<statementNumber, variable> pair = std::make_pair(x, numLHSMap[x]);
    results.insert(pair);
  }
  return results;
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash>
assign_store::getAssignPairFull(partialMatch partial) {
  auto results = std::unordered_set<std::pair<statementNumber, variable>, PairHash>();
  std::unordered_set<statementNumber> relevantStmt = reverseFullRHSMap[partial];
  for (auto const& x : relevantStmt) {
    std::pair<statementNumber, variable> pair = std::make_pair(x, numLHSMap[x]);
    results.insert(pair);
  }
  return results;
}

// get all Assign statements
std::unordered_set<statementNumber> assign_store::getAllAssigns() {
  std::unordered_set<statementNumber> assigns;
  for (auto const& x : this->numLHSMap) {
    assigns.insert(x.first);
  }
  return assigns;
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> assign_store::getAssignPair(partialMatch partial) {
  auto results = std::unordered_set<std::pair<statementNumber, variable>, PairHash>();
  std::unordered_set<statementNumber> relevantStmt = reverseNumRHSMap[partial];
  for (auto const& x : relevantStmt) {
    std::pair<statementNumber, variable> pair = std::make_pair(x, numLHSMap[x]);
    results.insert(pair);
  }
  return results;
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> assign_store::getAssignPair(Wildcard wildcard) {
  auto results = std::unordered_set<std::pair<statementNumber, variable>, PairHash>();
  std::unordered_set<statementNumber> relevantStmt = getAllAssigns();
  for (auto const& x : relevantStmt) {
    std::pair<statementNumber, variable> pair = std::make_pair(x, numLHSMap[x]);
    results.insert(pair);
  }
  return results;
}

std::unordered_set<statementNumber> assign_store::getAssignsWcF(Wildcard lhs, full rhs) {
  return reverseFullRHSMap[rhs];
}

std::unordered_set<statementNumber> assign_store::getAssignsFF(full lhs, full rhs) {
  std::unordered_set<statementNumber> results;
  std::unordered_set<statementNumber> relevantStmt = reverseFullRHSMap[rhs];
  for (auto const& x : relevantStmt) {
    if (numLHSMap[x] == lhs) {
      results.insert(x);
    }
  }
  return results;
}

std::unordered_set<statementNumber> assign_store::getAssigns(Wildcard lhs, partialMatch rhs) {
  return reverseNumRHSMap[rhs];
}

std::unordered_set<statementNumber> assign_store::getAssigns(Wildcard lhs, Wildcard rhs) {
  return getAllAssigns();
}

std::unordered_set<statementNumber> assign_store::getAssigns(partialMatch lhs, partialMatch rhs) {
  std::unordered_set<statementNumber> results;
  std::unordered_set<statementNumber> relevantStmt = reverseNumRHSMap[rhs];
  for (auto const& x : relevantStmt) {
    if (numLHSMap[x] == lhs) {
      results.insert(x);
    }
  }
  return results;
}

std::unordered_set<statementNumber> assign_store::getAssigns(partialMatch lhs, Wildcard rhs) {
  return reverseNumLHSMap[lhs];
}
