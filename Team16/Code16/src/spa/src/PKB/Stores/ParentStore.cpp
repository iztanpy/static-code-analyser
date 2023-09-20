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

    for (auto const& x : map) {
		std::unordered_set<statementNumber> childrens;
        for (auto const& y : x.second) {
			childrens.insert(y);
            if (this->ParentMap.find(y) != this->ParentMap.end()) {
				std::unordered_set<statementNumber> set = getChildrens(y);
				childrens.insert(set.begin(), set.end());
			}
		}
        if (childrens.size() > 0) {
			this->ParentStarMap[x.first] = childrens;
		}
	}

    for (auto const& x : this->ParentStarMap) {
        for (auto const& y : x.second) {
			this->ParentStarMapReverse[y].insert(x.first);
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

