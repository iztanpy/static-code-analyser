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

void NextStore::storeCfgLegend(std::unordered_map<statementNumber, CfgNode> cfgLegend) {
    this->cfgLegend = cfgLegend;
}

std::set<std::pair<statementNumber, statementNumber>> NextStore::Next(StmtEntity, StmtEntity);

std::set<statementNumber> NextStore::Next(StmtEntity, Wildcard);

std::set<statementNumber> NextStore::Next(StmtEntity, statementNumber);

std::set<statementNumber> NextStore::Next(Wildcard, StmtEntity);

std::set<statementNumber> NextStore::Next(statementNumber, StmtEntity);

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

std::set<std::pair<statementNumber, statementNumber>> NextStore::NextStar(StmtEntity, StmtEntity);

std::set<statementNumber> NextStore::NextStar(StmtEntity, Wildcard);

std::set<statementNumber> NextStore::NextStar(StmtEntity, statementNumber);

std::set<statementNumber> NextStore::NextStar(Wildcard, StmtEntity);

std::set<statementNumber> NextStore::NextStar(statementNumber, StmtEntity);

bool NextStore::NextStar(Wildcard, Wildcard);

bool NextStore::NextStar(Wildcard, statementNumber);

bool NextStore::NextStar(statementNumber, Wildcard);

bool NextStore::NextStar(statementNumber, statementNumber);