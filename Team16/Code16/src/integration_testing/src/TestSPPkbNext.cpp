//
// Created by Isaac Tan on 19/10/23.
//

#include <unordered_set>
#include "catch.hpp"
#include "PKB/Stores/next_store.h"
#include "utils/entity_types.h"
#include "SP/SourceProcessor.h"
#include "PKB/API/write_facade.h"
#include "PKB/pkb.h"
#include "PKB/API/read_facade.h"
#include "qps/qps.h"

TEST_CASE("Test Next store") {
  auto nextStore = next_store();
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  write_facade writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  QPS qps(readFacade);

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
  writeFacade.storeNext(sourceProcessor.getNextStatementMap());
  // same node
  REQUIRE(readFacade.Next(StmtEntity::kIf, Wildcard()) == std::unordered_set<statementNumber>({1, 3}));
  REQUIRE(readFacade.Next(Wildcard(), StmtEntity::kIf) == std::unordered_set<statementNumber>({3}));
  REQUIRE(readFacade.Next(1, StmtEntity::kIf) == std::unordered_set<statementNumber>({3}));
  REQUIRE(readFacade.Next(StmtEntity::kIf, 4) == std::unordered_set<statementNumber>({3}));
}

TEST_CASE("Test Next Star") {
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
  auto map = sourceProcessor.getNextStatementMap();
  writeFacade.storeCfgLegend(cfgLegend);
  writeFacade.storeNext(map);
  // same node
  REQUIRE(readFacade.NextStar(StmtEntity::kIf, Wildcard()) == std::unordered_set<statementNumber>({1, 4}));
  REQUIRE(readFacade.NextStar(1, StmtEntity::kIf) == std::unordered_set<statementNumber>({4}));
  REQUIRE(readFacade.NextStar(Wildcard(), StmtEntity::kIf) == std::unordered_set<statementNumber>({4}));
  REQUIRE(readFacade.NextStar(StmtEntity::kIf, 9) == std::unordered_set<statementNumber>({1, 4}));
  REQUIRE(readFacade.NextStar(StmtEntity::kIf, StmtEntity::kStmt)
              == std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>(
                  {{1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {1, 9}, {4, 5}, {4, 6}, {4, 7}, {4, 8},
                   {4, 9}}));
}

//
//TEST_CASE("Test Next Star 2") {
//    auto nextStore = next_store();
//    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//    auto writeFacade = write_facade(*pkb_ptr);
//    auto sourceProcessor = SourceProcessor(&writeFacade);
//    auto readFacade = ReadFacade(*pkb_ptr);
//    std::string simpleProgram4 = R"(procedure Second {
//    while (x==0) {
//        if (x==1) then {
//            while (x==0) {
//                help = help +1;
//            }
//        } else {
//            if (x == 2) then {
//                 help = help +1;
//                 help = help +2;
//                 help = help +3;
//            }
//            test = test + 1;
//        }
//        a = a + b;
//      }
//      })";
//    sourceProcessor.processSource(simpleProgram4);
//    std::unordered_map<int, std::shared_ptr<CfgNode> > cfgLegend = sourceProcessor.getStmtNumberToCfgNodeHashmap();
//    auto map = sourceProcessor.getNextStatementMap();
//    writeFacade.storeCfgLegend(cfgLegend);
//    writeFacade.storeNext(map);
//    REQUIRE(readFacade.NextStar(StmtEntity::kIf, StmtEntity::kStmt) == std::set<std::pair<statementNumber, statementNumber>>(
//            {{2,1}, {2,2}, {2,3}, {2,4}, {2,5}, {2,6}, {2,7}, {2,8}, {2,9}, {2,10},
//             {5,1}, {5,2}, {5,3}, {5,4}, {5,5}, {5,6}, {5,7}, {5,8}, {5,9}, {5,10}}));
//}
//
