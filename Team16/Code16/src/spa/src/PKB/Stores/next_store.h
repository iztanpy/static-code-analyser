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
#include <queue>

#include "SP/sp_cfg/cfg.h"
#include "SP/sp_cfg/cfg_node.h"
#include "utils/entity_types.h"
#include "utils/clauses_types.h"
#include "utils/hash_utils.h"

class next_store {
 private:
  typedef std::string variable;
  typedef int statementNumber;
  std::unordered_map<std::string, std::shared_ptr<cfg_node>> cfgRoots;
  std::unordered_map<statementNumber, std::shared_ptr<cfg_node>> cfgLegend;
  std::unordered_map<statementNumber, std::unordered_set<std::shared_ptr<cfg_node>>> NextStarMap;
  std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap;
  std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMapReverse;

 public:
  next_store();

  void storeNext(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap);

  void storeCfg(std::unordered_map<std::string, std::shared_ptr<cfg_node>> cfgRoots);

  void storeCfgLegend(std::unordered_map<statementNumber, std::shared_ptr<cfg_node>> cfgLegend);

  bool isNext(Wildcard, Wildcard);

  bool isNext(Wildcard, statementNumber num);

  bool isNext(statementNumber num, Wildcard);

  bool isNext(statementNumber num1, statementNumber num2);

  bool isNextStar(Wildcard, Wildcard);

  bool isNextStar(Wildcard, statementNumber num);

  bool isNextStar(statementNumber num, Wildcard);

  bool isNextStar(statementNumber num1, statementNumber num2);

  bool isNodeFollowing(std::shared_ptr<cfg_node> startNode,
                       std::shared_ptr<cfg_node> endNode,
                       std::unordered_set<std::shared_ptr<cfg_node>> visitedNodes,
                       std::unordered_set<statementNumber> visitedNums);

  std::unordered_set<statementNumber> getNext(statementNumber num);

  std::unordered_set<statementNumber> getNextReverse(statementNumber num);

  void initialiseNextStar();

  void clearCache();
};

