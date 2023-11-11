//
// Created by Isaac Tan on 17/9/23.
//

#include "ParentStore.h"
#include <stack>

ParentStore::ParentStore() {
  std::unordered_map<statementNumber, std::unordered_set<statementNumber>> ParentMap;
  std::unordered_map<statementNumber, statementNumber> ParentMapReverse;
  std::unordered_map<statementNumber, std::unordered_set<statementNumber>> ParentStarMap;
  std::unordered_map<statementNumber, std::unordered_set<statementNumber>> ParentStarMapReverse;
}

void ParentStore::storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map) {
  this->ParentMap = map;

  for (auto const& x : map) {
    for (auto const& y : x.second) {
      this->ParentMapReverse[y] = x.first;
    }
  }

  for (const auto& [node, children] : map) {
    std::unordered_set<statementNumber> visited;
    std::stack<statementNumber> stack;
    for (statementNumber child : children) {
      stack.push(child);
    }

    while (!stack.empty()) {
      statementNumber current = stack.top();
      stack.pop();

      if (visited.find(current) != visited.end()) {
        continue;
      }

      ParentStarMap[node].insert(current);
      visited.insert(current);

      if (map.find(current) == map.end()) {
        continue;
      }

      for (statementNumber child : map.at(current)) {
        if (visited.find(child) == visited.end()) {
          stack.push(child);
        }
      }
    }
  }

  for (const auto& [node, children] : ParentStarMap) {
    for (statementNumber child : children) {
      ParentStarMapReverse[child].insert(node);
    }
  }
}

std::unordered_set<ParentStore::statementNumber> ParentStore::getChildren(statementNumber statement) {
  std::unordered_set<statementNumber> children = this->ParentMap[statement];
  return children;
}

std::unordered_set<ParentStore::statementNumber> ParentStore::getParent(statementNumber statement) {
  statementNumber parent = this->ParentMapReverse[statement];
  if (parent == 0) {
    return std::unordered_set<ParentStore::statementNumber>{};
  }
  return std::unordered_set<ParentStore::statementNumber>{parent};
}

bool ParentStore::isParent(statementNumber parent, statementNumber child) {
  return this->ParentMap[parent].find(child) != this->ParentMap[parent].end();
}

bool ParentStore::isParent(statementNumber parent, Wildcard wildcard) {
  return this->ParentMap[parent].size() > 0;
}

bool ParentStore::isParent(Wildcard wildcard, statementNumber child) {
  return this->ParentMapReverse[child] != 0;
}

bool ParentStore::isParent(Wildcard wildcard, Wildcard wildcard2) {
  return this->ParentMap.size() > 0;
}

bool ParentStore::isParentStar(statementNumber parent, Wildcard wildcard) {
  return this->ParentStarMap[parent].size() > 0;
}

bool ParentStore::isParentStar(Wildcard wildcard, statementNumber child) {
  return this->ParentStarMapReverse[child].size() > 0;
}

bool ParentStore::isParentStar(Wildcard wildcard, Wildcard wildcard2) {
  return this->ParentStarMap.size() > 0;
}

bool ParentStore::isParentStar(statementNumber parent, statementNumber child) {
  std::unordered_set<statementNumber> childrens = getChildrens(parent);
  return childrens.find(child) != childrens.end();
}

std::unordered_set<ParentStore::statementNumber> ParentStore::getChildrens(statementNumber statement) {
  return this->ParentStarMap[statement];
}

std::unordered_set<ParentStore::statementNumber> ParentStore::getParents(statementNumber statement) {
  return this->ParentStarMapReverse[statement];
}
