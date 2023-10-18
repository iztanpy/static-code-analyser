//
// Created by Isaac Tan on 18/10/23.
//

#pragma once

#include <string>
#include <iostream>

#include <unordered_set>
#include <unordered_map>
#include <set>

#include "SP/sp_cfg/Cfg.h"
#include "SP/sp_cfg/CfgNode.h"
#include "utils/entity_types.h"
#include "utils/clauses_types.h"



class NextStore {
private:
    typedef std::string variable;
    typedef int statementNumber;
    std::unordered_map<statementNumber, CfgNode> cfgLegend;
    Cfg cfg;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMapReverse;

public:
    NextStore();

    void storeNext(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap);

    void storeCfg(Cfg cfg);

    void storeCfgLegend(std::unordered_map<statementNumber, CfgNode> cfgLegend);

    std::set<std::pair<statementNumber, statementNumber>> Next(StmtEntity ent1, StmtEntity ent2);

    std::set<statementNumber> Next(StmtEntity, Wildcard);

    std::set<statementNumber> Next(StmtEntity ent, statementNumber num);

    std::set<statementNumber> Next(Wildcard, StmtEntity ent);

    std::set<statementNumber> Next(statementNumber num, StmtEntity ent);

    bool isNext(Wildcard, Wildcard);

    bool isNext(Wildcard, statementNumber num);

    bool isNext(statementNumber num, Wildcard);

    bool isNext(statementNumber num1, statementNumber num2);

    std::set<std::pair<statementNumber, statementNumber>> NextStar(StmtEntity, StmtEntity);

    std::set<statementNumber> NextStar(StmtEntity, Wildcard);

    std::set<statementNumber> NextStar(StmtEntity, statementNumber);

    std::set<statementNumber> NextStar(Wildcard, StmtEntity);

    std::set<statementNumber> NextStar(statementNumber, StmtEntity);

    bool NextStar(Wildcard, Wildcard);

    bool NextStar(Wildcard, statementNumber);

    bool NextStar(statementNumber, Wildcard);

    bool NextStar(statementNumber, statementNumber);
};

