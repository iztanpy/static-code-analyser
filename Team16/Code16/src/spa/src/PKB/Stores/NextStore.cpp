#include "NextStore.h"
#include "SP/sp_cfg/Cfg.h"
#include "SP/sp_cfg/CfgNode.h"
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
    auto NextStarMap = std::unordered_map<statementNumber, std::unordered_set<statementNumber>>();
    auto NextStarMapReverse = std::unordered_map<statementNumber, std::unordered_set<statementNumber>>();
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
    // cache
    // for each element in cfgLegend
    if (NextStarMap.find(num1) != NextStarMap.end()) {
        if (NextStarMap[num1].find(num2) != NextStarMap[num1].end()) {
            return true;
        }
    }
    std::shared_ptr<CfgNode> startNode = cfgLegend[num1];
    std::shared_ptr<CfgNode> endNode = cfgLegend[num2];

    if (startNode == nullptr || endNode == nullptr) {
        return false;
    }

    for (auto it = cfgRoots.begin(); it != cfgRoots.end(); ++it) {
        auto visitedNums = std::unordered_set<statementNumber>();
        for (auto i: it->second->getStmtNumberSet()) {
            NextStarMap[i] = std::unordered_set<statementNumber>();
            // add visitedNums to NextStarMap[i]
            for (auto j: visitedNums) {
                NextStarMap[i].insert(j);
            }
            visitedNums.insert(i);
        }
        // 'it' is an iterator that points to a key-value pair
        auto node = it->second;
        // populate the whole NextStarMap
        auto fakeEndPoint = std::make_shared<CfgNode>();
        isNodeFollowing(node,
                        fakeEndPoint,
                        std::unordered_set<std::shared_ptr<CfgNode>>(),
                                std::unordered_set<statementNumber>());
    }

    if (NextStarMap.find(num1) != NextStarMap.end()) {
        if (NextStarMap[num1].find(num2) != NextStarMap[num1].end()) {
            return true;
        }
    }
    // check the cache to see if the statement numbers are inside
//    auto visited = std::unordered_set<std::shared_ptr<CfgNode>>();
//    auto visitedNums = std::unordered_set<statementNumber>();
//    if (isNodeFollowing(startNode, endNode, visited, visitedNums)) {
//        return true;
//    }
    return false;
}

bool NextStore::isNodeFollowing(std::shared_ptr<CfgNode> startNode,
                                std::shared_ptr<CfgNode> endNode,
                                std::unordered_set<std::shared_ptr<CfgNode> > visited,
                                std::unordered_set<statementNumber> visitedNums) {
    // start cache

    // traverse the entire cfgLegend and populate the NextStarMap

    if (startNode == nullptr || endNode == nullptr) {
        return false;
    }

    // if the startnode is the endnode, and we have visited the startnode before, this is a while loop
    if (startNode == endNode && visited.count(startNode) != 0) {
        return true;
    }
    // else if the endnode is not the startnode, but we have traversed the whole tree, then we return false
    if (visited.count(startNode) != 0) {
        return false;
    }
    // else we have not visited the startnode before, and we add it to the visited list
    visited.insert(startNode);

    // CACHING

    // there exists a path from all the visited nodes to the statement number list that we specified.
    std::set<statementNumber> nodeStatementNumberList = startNode->getStmtNumberSet();
    for (auto num : visitedNums) {
        for (auto nodeNum : nodeStatementNumberList) {
            NextStarMap[num].insert(nodeNum);
        }
    }
    // add the statementNumberList to the visited Nums
    for (auto num : nodeStatementNumberList) {
        visitedNums.insert(num);
    }

    std::set<std::shared_ptr<CfgNode>> childrens = startNode->getChildren();
    if (childrens.count(endNode) > 0) {
        return true;
    }
    // we want to keep track of the nodes that we have visited
    for (auto child : childrens) {
        if (isNodeFollowing(child, endNode, visited, visitedNums)) {
            return true;
        }
    }
    return false;
}

std::unordered_set<statementNumber> NextStore::getNext(statementNumber num) {
    if (NextMap.find(num) != NextMap.end()) {
        return NextMap[num];
    }
    return std::unordered_set<statementNumber>();
}

std::unordered_set<statementNumber> NextStore::getNextReverse(statementNumber num) {
    if (NextMapReverse.find(num) != NextMapReverse.end()) {
        return NextMapReverse[num];
    }
    return std::unordered_set<statementNumber>();
}

void NextStore::clearCache() {
    NextStarMap.clear();
    NextStarMapReverse.clear();
}
