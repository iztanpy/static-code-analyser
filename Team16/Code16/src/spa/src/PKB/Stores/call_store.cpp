#include "PKB/Stores/call_store.h"
#include <stack>
#include <iostream>
#include "utils/Error.h"

typedef int statementNumber;
typedef std::string procedure;

call_store::call_store() {
  this->callTable = std::unordered_map<procedure, std::unordered_set<procedure>>();
  this->callTableReverse = std::unordered_map<procedure, std::unordered_set<procedure>>();
  this->callTableStar = std::unordered_map<procedure, std::unordered_set<procedure>>();
  this->callTableStarReverse = std::unordered_map<procedure, std::unordered_set<procedure>>();
  this->callPairs = std::unordered_set<std::pair<statementNumber, procedure>, PairHash>();
}

void call_store::storeCalls(const std::unordered_map<procedure, std::unordered_set<procedure>> callTable) {
  this->callTable = callTable;

  for (auto const& x : callTable) {
    for (auto const& y : x.second) {
      this->callTableReverse[y].insert(x.first);
    }
  }

  for (const auto& [node, children] : callTable) {
    std::unordered_set<procedure> visited;
    std::stack<procedure> stack;
    for (procedure child : children) {
      stack.push(child);
    }

    while (!stack.empty()) {
      procedure current = stack.top();
      stack.pop();

      if (visited.find(current) != visited.end()) {
        continue;
      }

      if (node == current) {
        throw InvalidSemanticError();
      }
      callTableStar[node].insert(current);
      visited.insert(current);

      if (callTable.find(current) == callTable.end()) {
        continue;
      }

      for (procedure child : callTable.at(current)) {
        if (visited.find(child) == visited.end()) {
          stack.push(child);
        }
      }
    }
  }

  for (const auto& [node, children] : callTableStar) {
    for (procedure child : children) {
      callTableStarReverse[child].insert(node);
    }
  }
}

void call_store::storeCallPairs(std::unordered_map<statementNumber, procedure> callStatements) {
  for (auto it = callStatements.begin(); it != callStatements.end(); ++it) {
    statementNumber stmtNum = it->first;
    procedure procName = it->second;
    this->callPairs.insert(std::make_pair(stmtNum, procName));
  }
}

std::unordered_set<std::pair<statementNumber, procedure>, PairHash> call_store::getCallPairs() {
  return this->callPairs;
}

std::unordered_set<procedure> call_store::getCallChildren(procedure p) {
  return this->callTable[p];
}

std::unordered_set<procedure> call_store::call(StmtEntity proc, Wildcard wildcard) {
  std::unordered_set<procedure> result;
  for (auto const& x : callTable) {
    if (x.second.size() > 0) {
      result.insert(x.first);
    }
  }
  return result;
}

std::unordered_set<procedure> call_store::call(Wildcard wildcard, StmtEntity proc) {
  std::unordered_set<procedure> result;
  for (auto const& x : callTableReverse) {
    if (x.second.size() > 0) {
      result.insert(x.first);
    }
  }
  return result;
}

bool call_store::isCall(Wildcard wildcard, Wildcard wildcard2) {
  return !callTable.empty();
}

bool call_store::isCall(Wildcard wildcard, procedure p) {
  return callTableReverse[p] != std::unordered_set<procedure>();
}

bool call_store::isCall(procedure p, Wildcard wildcard) {
  return callTable[p] != std::unordered_set<procedure>();
}

bool call_store::isCall(procedure p, procedure p2) {
  return callTable[p].find(p2) != callTable[p].end();
}

std::unordered_map<procedure, std::unordered_set<procedure>> call_store::getCallStar() {
  return this->callTableStar;
}

std::unordered_set<std::pair<procedure, procedure>, PairHash> call_store::call(StmtEntity proc1,
                                                                              StmtEntity proc2) {
  std::unordered_set<std::pair<procedure, procedure>, PairHash> pairs;
  for (auto const& x : callTable) {
    for (auto const& second : x.second) {
      pairs.insert(std::make_pair(x.first, second));
    }
  }
  return pairs;
}

std::unordered_set<procedure> call_store::getCallParents(procedure p) {
  return this->callTableReverse[p];
}

std::unordered_set<std::pair<procedure, procedure>, PairHash> call_store::callStar(StmtEntity proc1,
                                                                                  StmtEntity proc2) {
  std::unordered_set<std::pair<procedure, procedure>, PairHash> pairs;
  for (auto const& x : callTableStar) {
    for (auto const& second : x.second) {
      pairs.insert(std::make_pair(x.first, second));
    }
  }
  return pairs;
}

std::unordered_set<procedure> call_store::callStar(StmtEntity proc, Wildcard wildcard) {
  std::unordered_set<procedure> result;
  for (auto const& x : callTableStar) {
    if (x.second.size() > 0) {
      result.insert(x.first);
    }
  }
  return result;
}

std::unordered_set<procedure> call_store::callStar(Wildcard wildcard, StmtEntity proc) {
  std::unordered_set<procedure> result;
  for (auto const& x : callTableStarReverse) {
    if (x.second.size() > 0) {
      result.insert(x.first);
    }
  }
  return result;
}

std::unordered_set<procedure> call_store::getCallStarChildren(procedure p) {
  return this->callTableStar[p];
}

std::unordered_set<procedure> call_store::getCallStarParents(procedure p) {
  return this->callTableStarReverse[p];
}

bool call_store::isCallStar(Wildcard wildcard, Wildcard wildcard2) {
  return !callTableStar.empty();
}

bool call_store::isCallStar(Wildcard wildcard, procedure p) {
  return callTableStarReverse[p] != std::unordered_set<procedure>();
}

bool call_store::isCallStar(procedure p, Wildcard wildcard) {
  return callTableStar[p] != std::unordered_set<procedure>();
}

bool call_store::isCallStar(procedure p, procedure p2) {
  return callTableStar[p].find(p2) != callTableStar[p].end();
}
