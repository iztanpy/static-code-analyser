#include "NextStore.h"
#include "SP/sp_cfg/Cfg.h"
#include "SP/sp_cfg/CfgNode.h"
#include <queue>
#include <unordered_set>
#include "utils/hash_utils.h"
//
// Created by Isaac Tan on 18/10/23.
//

typedef std::string variable;
typedef int statementNumber;

NextStore::NextStore() {
    std::unordered_map<std::string, std::shared_ptr<CfgNode>> cfgRoots;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMapReverse;
    std::unordered_map<statementNumber, std::shared_ptr<CfgNode>> cfgLegend;
    auto NextStarMap = std::unordered_map<statementNumber,
    std::unordered_set<std::shared_ptr<CfgNode>>>();
}

void NextStore::storeNext(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap) {
    this->NextMap = NextMap;
    for (auto const& x : NextMap) {
        for (auto const& y : x.second) {
        this->NextMapReverse[y].insert(x.first);
        }
    }
}

void NextStore::storeCfg(std::unordered_map<std::string, std::shared_ptr<CfgNode>> cfgRoots) {
    this->cfgRoots = cfgRoots;
}

void NextStore::storeCfgLegend(std::unordered_map<statementNumber, std::shared_ptr<CfgNode>> cfgLegend) {
    this->cfgLegend = cfgLegend;
}

bool NextStore::isNext(Wildcard, Wildcard) {
    if (this->NextMap.empty()) {
        return false;
    }
    return true;
}

// return true if statement number that is after anything
bool NextStore::isNext(Wildcard, statementNumber num) {
    if (NextMapReverse.find(num) != NextMapReverse.end()) {
        return true;
    }
    return false;
}

bool NextStore::isNext(statementNumber num, Wildcard) {
    if (NextMap.find(num) != NextMap.end()) {
        return true;
    }
    return false;
}

bool NextStore::isNext(statementNumber num1, statementNumber num2) {
    if (NextMap.find(num1) != NextMap.end()) {
        if (NextMap[num1].find(num2) != NextMap[num1].end()) {
            return true;
        }
    }
    return false;
}

bool NextStore::isNextStar(Wildcard, Wildcard) {
    return this->isNext(Wildcard(), Wildcard());
}

// first move to the statement number node
bool NextStore::isNextStar(Wildcard, statementNumber num) {
    // first locate the node using the nodelegend
    return this->isNext(Wildcard(), num);
}

bool NextStore::isNextStar(statementNumber num, Wildcard) {
    return this->isNext(num, Wildcard());
}

bool NextStore::isNextStar(statementNumber num1, statementNumber num2) {
    if (NextStarMap.empty()) {
        initialiseNextStar();
    }

    // if they are in the same node and num1 is before num 2 this is true
    if (cfgLegend[num1] == cfgLegend[num2] && cfgLegend[num1] != nullptr) {
        auto stmtNums = cfgLegend[num1]->getStmtNumberSet();
        auto it = stmtNums.find(num1);
        auto it2 = stmtNums.find(num2);
        if (*it < *it2) {
            return true;
        } else {
            // check if node1 can be reached from itself
            if (NextStarMap[num1].find(cfgLegend[num1]) != NextStarMap[num1].end()) {
                return true;
            }
            return false;
        }
    }
    // for each element in cfgLegend
    if (NextStarMap.find(num1) != NextStarMap.end()) {
        if (NextStarMap[num1].find(cfgLegend[num2]) != NextStarMap[num1].end()) {
            return true;
        }
    }
    return false;
}

struct CustomComparator {
    bool operator()(const std::pair<int, std::unordered_set<int>>& a, const std::pair<int, std::unordered_set<int>>& b) {
        return a.second.size() < b.second.size(); // Reverse the comparison
    }
};

void NextStore::initialiseNextStar() {
    for (auto it = cfgRoots.begin(); it != cfgRoots.end(); ++it) {
        auto firstNode = it->second;
        auto firstNum = firstNode->getStmtNumberSet();
        std::stack<
                std::pair<std::shared_ptr<CfgNode>, std::set<int>>
                > stack;
        stack.push(std::make_pair(firstNode, firstNum));
        while (!stack.empty()) {
            auto currentNode = stack.top().first;
            auto visited = stack.top().second;
            stack.pop();
            auto nextNodes = currentNode->getChildren();
            for (auto node : nextNodes) {
                // insert the visited list into the hashmap
                bool changed = false;
                for (auto num : visited) {
                    auto pink = NextStarMap[num];
                    if (NextStarMap[num].find(node) == NextStarMap[num].end()) {
                        NextStarMap[num].insert(node);
                        changed = true;
                    }
                }
                if (changed) {
                    std::set<int> newVisited(visited);
                    for (auto num : node->getStmtNumberSet()) {
                        newVisited.insert(num);
                    }
                    stack.push(std::make_pair(node, newVisited));
                }
            }
        }
    }
}

std::unordered_set<statementNumber> NextStore::getNext(statementNumber num) {
    if (NextMap.find(num) != NextMap.end()) {
        return NextMap[num];
    }
    return {};
}

std::unordered_set<statementNumber> NextStore::getNextReverse(statementNumber num) {
    if (NextMapReverse.find(num) != NextMapReverse.end()) {
        return NextMapReverse[num];
    }
    return std::unordered_set<statementNumber>();
}

void NextStore::clearCache() {
    NextStarMap.clear();
}
