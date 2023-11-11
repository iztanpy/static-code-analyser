//
// Created by Isaac Tan on 13/10/23.
//

#include "catch.hpp"
#include <memory>
#include <unordered_set>
#include <string>

#include "utils/hash_utils.h"
#include "SP/SourceProcessor.h"
#include "PKB/API/write_facade.h"
#include "PKB/API/read_facade.h"
#include "qps/qps.h"

TEST_CASE("TEST Simple If") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    std::string simpleProgram = "procedure foo { if (number > 0) then { if (x == 1) then { x = x + 1; } } else { x = x + 1; } }";

    sourceProcessor.processSource(simpleProgram);

    REQUIRE(readFacade.getAllIf() == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({{1, "number"}, {2, "x"}}));
    REQUIRE(readFacade.getIf("number") == std::unordered_set<statementNumber>({1}));
    REQUIRE(readFacade.getIf("x") == std::unordered_set<statementNumber>({2}));
    REQUIRE(readFacade.getIf(Wildcard()) == std::unordered_set<statementNumber>({1, 2}));
}