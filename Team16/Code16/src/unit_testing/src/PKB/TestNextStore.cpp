//
// Created by Isaac Tan on 19/10/23.
//
#include <unordered_set>
#include "catch.hpp"
#include "PKB/Stores/next_store.h"
#include "utils/entity_types.h"
#include "SP/SourceProcessor.h"
#include "PKB/API/write_facade.h"
#include "PKB/API/read_facade.h"

TEST_CASE("Test Next store") {
  auto nextStore = next_store();
  nextStore.storeNext({{1, {2, 3}}, {2, {4, 5}}, {3, {6, 7}}, {6, {8, 9}}});
  REQUIRE(nextStore.isNext(1, 2));
  REQUIRE(nextStore.isNext(Wildcard(), 2));
  REQUIRE(nextStore.isNext(1, Wildcard()));
  REQUIRE(nextStore.isNext(Wildcard(), Wildcard()));
  REQUIRE(!nextStore.isNext(1, 4));
  REQUIRE(!nextStore.isNext(9, Wildcard()));
  REQUIRE(!nextStore.isNext(Wildcard(), 1));
}

TEST_CASE("Test NextStar Store if") {
  auto nextStore = next_store();
  auto pkb = PKB();
  auto facade = write_facade(pkb);
  auto sourceProcessor = SourceProcessor(&facade);
  std::string simpleProgram4 = R"(procedure Second {
        if (x==1) then {
            help = help +1;
        } else {
            if (x == 2) then {
                 help = help +1;
                 help = help +2;
                 help = help +3;
            }
            test = test + 1;
        }
        a = a + b;
      })";
  sourceProcessor.processSource(simpleProgram4);
  std::unordered_map<int, std::shared_ptr<CfgNode> > cfgLegend = sourceProcessor.getStmtNumberToCfgNodeHashmap();
  auto map = sourceProcessor.getNextStatementMap();
  nextStore.storeNext(map);
  nextStore.storeCfgLegend(cfgLegend);
  nextStore.storeCfg(sourceProcessor.getCfgNodesMap());
  // same node
  REQUIRE(nextStore.isNextStar(1, 2));
  // different node
  REQUIRE(nextStore.isNextStar(1, 3));
  // no path from 2 to 6
  REQUIRE(!nextStore.isNextStar(2, 6));
  //path from 2 to 8
  REQUIRE(nextStore.isNextStar(2, 8));
  // line number 9 does not exist
  REQUIRE(!nextStore.isNextStar(2, 9));
  // line number 10 does not exist
  REQUIRE(!nextStore.isNextStar(10, 5));
  // both line numbers do not exist
  REQUIRE(!nextStore.isNextStar(12, 14));
}

TEST_CASE("Test NextStar Store while") {
  auto nextStore = next_store();
  auto pkb = PKB();
  auto facade = write_facade(pkb);
  auto sourceProcessor = SourceProcessor(&facade);
  std::string simpleProgram4 = R"(procedure Second {
        while (x==1) {
            help = help +1;
        }
        a = a + b;
      })";
  sourceProcessor.processSource(simpleProgram4);
  std::unordered_map<int, std::shared_ptr<CfgNode> > cfgLegend = sourceProcessor.getStmtNumberToCfgNodeHashmap();
  nextStore.storeNext(sourceProcessor.getNextStatementMap());
  nextStore.storeCfgLegend(cfgLegend);
  nextStore.storeCfg(sourceProcessor.getCfgNodesMap());
  // same node
  REQUIRE(nextStore.isNextStar(1, 2));
  // different node
  REQUIRE(nextStore.isNextStar(1, 3));
  // returns to the while loop
  REQUIRE(nextStore.isNextStar(2, 1));
  // no path from 3 to 1
  REQUIRE(!nextStore.isNextStar(3, 1));
  // line number 4 does not exist
  REQUIRE(!nextStore.isNextStar(3, 4));
  // line number 8 and 9 does not exist
  REQUIRE(!nextStore.isNextStar(8, 9));
  // line number 15
  REQUIRE(!nextStore.isNextStar(15, 3));
}

TEST_CASE("If else in a while loop") {
  auto nextStore = next_store();
  auto pkb = PKB();
  auto facade = write_facade(pkb);
  auto sourceProcessor = SourceProcessor(&facade);
  std::string simpleProgram4 = R"(procedure Second {
        while (x==0) {
            if (x==1) then {
                help = help +1;
            } else {
                if (x == 2) then {
                     help = help +1;
                     help = help +2;
                     help = help +3;
                }
                test = test + 1;
            }
        }
        a = a + b;
      })";
  sourceProcessor.processSource(simpleProgram4);
  std::unordered_map<int, std::shared_ptr<CfgNode> > cfgLegend = sourceProcessor.getStmtNumberToCfgNodeHashmap();
  auto map = sourceProcessor.getNextStatementMap();
  nextStore.storeNext(map);
  nextStore.storeCfgLegend(cfgLegend);
  nextStore.storeCfg(sourceProcessor.getCfgNodesMap());
  // same node
  REQUIRE(nextStore.isNextStar(2, 3));
}

TEST_CASE("While in a if else") {
  auto nextStore = next_store();
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  auto sourceProcessor = SourceProcessor(&writeFacade);
  auto readFacade = ReadFacade(*pkb_ptr);
  std::string simpleProgram4 = R"(procedure Second {
        if (x==1) then {
            while (x==0) {
                help = help +1;
            }
        } else {
            if (x == 2) then {
                 help = help +1;
                 help = help +2;
                 help = help +3;
            }
            test = test + 1;
        }
        a = a + b;
      })";
  sourceProcessor.processSource(simpleProgram4);
  std::unordered_map<int, std::shared_ptr<CfgNode> > cfgLegend = sourceProcessor.getStmtNumberToCfgNodeHashmap();
  nextStore.storeNext(sourceProcessor.getNextStatementMap());
  nextStore.storeCfgLegend(cfgLegend);
  nextStore.storeCfg(sourceProcessor.getCfgNodesMap());
  // same node
  REQUIRE(nextStore.isNextStar(2, 3));
  // node is not reachable
  REQUIRE(!nextStore.isNextStar(3, 6));
  REQUIRE(!nextStore.isNextStar(2, 6));
  REQUIRE(!nextStore.isNextStar(2, 5));
  // node is reachable
  REQUIRE(nextStore.isNextStar(2, 2));
  REQUIRE(nextStore.isNextStar(1, 6));
  REQUIRE(nextStore.isNextStar(1, 9));
}




