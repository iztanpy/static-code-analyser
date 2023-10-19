//
// Created by Isaac Tan on 18/10/23.
//

#include "NextStore.h"

typedef std::string variable;
typedef int statementNumber;

NextStore::NextStore() {
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMapReverse;
}

void NextStore::storeNext(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap) {
    this->NextMap = NextMap;
    for (auto const& x : NextMap) {
        for (auto const& y : x.second) {
        this->NextMapReverse[y].insert(x.first);
        }
    }
}

void NextStore::storeCfg(Cfg cfg) {
    this->cfg = cfg;
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
    std::shared_ptr<CfgNode> startNode = cfgLegend[num];
    // check if node statement number list has any number positioned before num
    std::set nodeStatementNumberList = startNode->getStmtNumberSet();
    // if num is not at the start of the list, there are other numbers before it
    if (nodeStatementNumberList.find(num) != nodeStatementNumberList.begin()) {
        return true;
    }
    // if num has a parent, there are other numbers beforee it
    if (startNode->getParentNode(num) != nullptr) {
        return true;
    }
    // else this is not valid
    return false;
}

bool NextStore::isNextStar(statementNumber num, Wildcard) {
    std::shared_ptr<CfgNode> startNode = cfgLegend[num];
    std::set nodeStatementNumberList = startNode->getStmtNumberSet();
    // if num is not at the end of the list, there are other numbers after it
    if (nodeStatementNumberList.find(num) != nodeStatementNumberList.end()) {
        return true;
    }
    // if num has a child, there are other numbers after it
    if (startNode->getChildren().size() != 0) {
        return true;
    }
    // else this is not valid
    return false;
}

bool NextStore::isNextStar(statementNumber num1, statementNumber num2) {
    std::shared_ptr<CfgNode> startNode = cfgLegend[num1];
    std::shared_ptr<CfgNode> endNode = cfgLegend[num2];

    if (startNode == nullptr || endNode == nullptr) {
        return false;
    }

    if (startNode == endNode) {
        // check the statement list to determine which number comes first
        std::set nodeStatementNumberList = startNode->getStmtNumberSet();
        if (*nodeStatementNumberList.find(num1) < *nodeStatementNumberList.find(num2)) {
            return true;
        }
    }
    auto visited = std::unordered_set<std::shared_ptr<CfgNode>>();
    if (isNodeFollowing(startNode, endNode, visited)) {
        return true;
    }
    return false;
}

bool NextStore::isNodeFollowing(std::shared_ptr<CfgNode> startNode,
                                std::shared_ptr<CfgNode> endNode,
                                std::unordered_set<std::shared_ptr<CfgNode>>visited) {
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
    std::set<std::shared_ptr<CfgNode>> childrens = startNode->getChildren();
    if (childrens.count(endNode) > 0) {
        return true;
    }
    // we want to keep track of the nodes that we have visited
    for (auto child : childrens) {
        if (isNodeFollowing(child, endNode, visited)) {
            return true;
        }
    }
    return false;
};