//
// Created by Isaac Tan on 17/9/23.
//

#include "ParentStore.h"
#include <iostream>

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

    // ai-gen start (gpt3, 1)
    for (const auto& [node, children] : ParentMap) {
        std::unordered_set<int> visited;
        std::unordered_set<int> stack;
        visited.insert(node);
        stack.insert(children.begin(), children.end());

        while (!stack.empty()) {
            int current = *stack.begin();
            stack.erase(stack.begin());

            if (visited.find(current) == visited.end()) {
                ParentStarMap[node].insert(current);
                visited.insert(current);
                for (int child : ParentMap[current]) {
                    if (visited.find(child) == visited.end()) {
                        stack.insert(child);
                    }
                }
            }
        }
    }
    for (const auto& [node, children] : ParentStarMap) {
        for (int child : children) {
            ParentStarMapReverse[child].insert(node);
        }
    }



    // ai-gen end
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

