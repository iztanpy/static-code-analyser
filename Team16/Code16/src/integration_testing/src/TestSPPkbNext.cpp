//
// Created by Isaac Tan on 19/10/23.
//

#include <unordered_set>
#include "catch.hpp"
#include "PKB/Stores/NextStore.h"
#include "utils/entity_types.h"
#include "SP/SourceProcessor.h"
#include "PKB/API/WriteFacade.h"
#include "PKB/PKB.h"
#include "PKB/API/ReadFacade.h"
#include "qps/qps.h"

TEST_CASE("Test Next store") {
    auto nextStore = NextStore();
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
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
    REQUIRE(readFacade.Next(StmtEntity::kIf, Wildcard()) == std::set<statementNumber>({2,4}));
    REQUIRE(readFacade.Next(Wildcard(), StmtEntity::kIf) == std::set<statementNumber>({1}));
    REQUIRE(readFacade.Next(1, StmtEntity::kIf) == std::set<statementNumber>({3}));
}

TEST_CASE("Test NextStar store") {
    auto nextStore = NextStore();
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
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
    REQUIRE(readFacade.Next(StmtEntity::kIf, Wildcard()) == std::set<statementNumber>({2,4}));
    REQUIRE(readFacade.Next(Wildcard(), StmtEntity::kIf) == std::set<statementNumber>({1}));
    REQUIRE(readFacade.Next(1, StmtEntity::kIf) == std::set<statementNumber>({3}));
}

TEST_CASE("Test Next Star") {
    auto nextStore = NextStore();
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

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
    writeFacade.storeCfgLegend(cfgLegend);
    // same node
    REQUIRE(readFacade.NextStar(StmtEntity::kIf, Wildcard()) == std::set<statementNumber>({2,4}));
    REQUIRE(readFacade.NextStar(Wildcard(), StmtEntity::kIf) == std::set<statementNumber>({1}));
    REQUIRE(readFacade.NextStar(1, StmtEntity::kIf) == std::set<statementNumber>({3}));

}