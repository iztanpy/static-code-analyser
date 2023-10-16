//
// Created by Isaac Tan on 13/10/23.
//

#include "catch.hpp"
#include <memory>
#include <unordered_set>
#include <string>

#include "utils/hash_utils.h"
#include "SP/SourceProcessor.h"
#include "PKB/API/WriteFacade.h"
#include "PKB/API/ReadFacade.h"
#include "qps/qps.h"

TEST_CASE("TEST Simple While") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    std::string simpleProgram = "procedure foo {  while (number > 0) { while (x > 0) { while (a > b) {x = x + 1; } } }}";

    sourceProcessor.processSource(simpleProgram);

    REQUIRE(readFacade.getAllWhile() == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({{1, "number"}, {2, "x"}, {3, "a"}, {3, "b"}}));
    REQUIRE(readFacade.getWhile("number") == std::unordered_set<statementNumber>({1}));
    REQUIRE(readFacade.getWhile("x") == std::unordered_set<statementNumber>({2}));
    REQUIRE(readFacade.getWhile("a") == std::unordered_set<statementNumber>({3}));
    REQUIRE(readFacade.getWhile("b") == std::unordered_set<statementNumber>({3}));
    REQUIRE(readFacade.getWhile(Wildcard()) == std::unordered_set<statementNumber>({1, 2, 3}));
}