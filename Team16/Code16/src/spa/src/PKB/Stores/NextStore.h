//
// Created by Isaac Tan on 18/10/23.
//

#pragma once

#include <string>
#include <iostream>

#include <unordered_set>
#include <unordered_map>
#include <set>
#include <memory>
#include <utility>
#include <stack>

#include "SP/sp_cfg/Cfg.h"
#include "SP/sp_cfg/CfgNode.h"
#include "utils/entity_types.h"
#include "utils/clauses_types.h"



class NextStore {
 private:
    typedef std::string variable;
    typedef int statementNumber;
    std::unordered_map<std::string, std::shared_ptr<CfgNode>> cfgRoots;
    std::unordered_map<statementNumber, std::shared_ptr<CfgNode>> cfgLegend;
    std::unordered_map<statementNumber, std::unordered_set<std::shared_ptr<CfgNode>>> NextStarMap;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMapReverse;

 public:
    NextStore();

    void storeNext(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap);

    void storeCfg(std::unordered_map<std::string, std::shared_ptr<CfgNode>> cfgRoots);

    void storeCfgLegend(std::unordered_map<statementNumber, std::shared_ptr<CfgNode>> cfgLegend);

    bool isNext(Wildcard, Wildcard);

    bool isNext(Wildcard, statementNumber num);

    bool isNext(statementNumber num, Wildcard);

    bool isNext(statementNumber num1, statementNumber num2);

    bool isNextStar(Wildcard, Wildcard);

    bool isNextStar(Wildcard, statementNumber num);

    bool isNextStar(statementNumber num, Wildcard);

    bool isNextStar(statementNumber num1, statementNumber num2);

    bool isNodeFollowing(std::shared_ptr<CfgNode> startNode,
                         std::shared_ptr<CfgNode> endNode,
                         std::unordered_set<std::shared_ptr<CfgNode>> visitedNodes,
                         std::unordered_set<statementNumber> visitedNums);

    std::unordered_set<statementNumber> getNext(statementNumber num);

    std::unordered_set<statementNumber> getNextReverse(statementNumber num);

    void initialiseNextStar();


    void clearCache();
};

