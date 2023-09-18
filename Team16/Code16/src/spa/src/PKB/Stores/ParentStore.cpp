//
// Created by Isaac Tan on 17/9/23.
//

#include "ParentStore.h"

ParentStore::ParentStore() {
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> ParentMap;
    std::unordered_map<statementNumber, statementNumber> ParentMapReverse;
}

void ParentStore::storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map) {
    this->ParentMap = map;
    for (auto const& x : map) {
        for (auto const& y : x.second) {
            this->ParentMapReverse[y] = x.first;
        }
    }
}

std::unordered_set<ParentStore::statementNumber> ParentStore::getChildren(statementNumber statement) {
    std::unordered_set<statementNumber> children = this->ParentMap[statement];
    return children;
}

ParentStore::statementNumber ParentStore::getParent(statementNumber statement) {
    statementNumber parent = this->ParentMapReverse[statement];
    return parent;
}

bool ParentStore::isParent(statementNumber parent, statementNumber child) {
    return this->ParentMap[parent].find(child) != this->ParentMap[parent].end();
}

std::unordered_set<ParentStore::statementNumber> ParentStore::getChildrens(statementNumber statement) {
    std::unordered_set<statementNumber> childrens;
    for (auto const& x : this->ParentMap[statement]) {
        childrens.insert(x);
        if (this->ParentMap.find(x) != this->ParentMap.end()) {
            std::unordered_set<statementNumber> set = getChildrens(x);
            childrens.insert(set.begin(), set.end());
        }
    }
    return childrens;
}

std::unordered_set<ParentStore::statementNumber> ParentStore::getParents(statementNumber statement) {
    std::unordered_set<statementNumber> parents;
    while (this->ParentMapReverse.find(statement) != this->ParentMapReverse.end()) {
        parents.insert(this->ParentMapReverse[statement]);
        statement = ParentMapReverse[statement];
    }
    return parents;
}

bool ParentStore::isParentStar(statementNumber parent, statementNumber child) {
    std::unordered_set<statementNumber> childrens = getChildrens(parent);
    return childrens.find(child) != childrens.end();
}

