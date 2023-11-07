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
    // for each element in cfgLegend
    if (NextStarMap.find(num1) != NextStarMap.end()) {
        if (NextStarMap[num1].find(num2) != NextStarMap[num1].end()) {
            return true;
        }
    }
    return false;
}

void NextStore::initialiseNextStar() {
    for (auto it = cfgRoots.begin(); it != cfgRoots.end(); ++it) {
        auto firstNum = *it->second->getStmtNumberSet().begin(); // first number of the first node
        std::stack<std::pair<statementNumber,std::unordered_set<statementNumber>>> stack;
        std::unordered_set<statementNumber> initial;
        initial.insert(firstNum);
        stack.push(std::make_pair(firstNum,initial));
        while (!stack.empty()) {
            statementNumber currentStatement = stack.top().first;
            auto visited = stack.top().second;
            stack.pop();
            auto nextStatements = getNext(currentStatement);
            for (auto nextStatement: nextStatements) {
                // insert the visited list into the hashmap
                bool changed = false;

                for (auto num: visited) {
                    if (NextStarMap[num].find(nextStatement) == NextStarMap[num].end()) {
                        NextStarMap[num].insert(nextStatement);
                        changed = true;
                    }
                }
                if (!changed) {
                    continue;
                }
                std::unordered_set<int> newVisited(visited);
                newVisited.insert(nextStatement);
                stack.push(std::make_pair(nextStatement,newVisited));
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
