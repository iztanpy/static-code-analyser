//
// Created by Isaac Tan on 17/9/23.
//

//
// Created by Isaac Tan on 17/9/23.
//

#include <unordered_set>
#include <unordered_map>
#include "catch.hpp"
#include "PKB/Stores/UsesStore.h"

typedef int statementNumber;
typedef std::string variable;


TEST_CASE("Test Uses Store Add") {
    auto usesStore = UsesStore();
    usesStore.addLineUsesVar(std::unordered_map<statementNumber, std::unordered_set<variable>>
    {{1, {"x"}}, {2, {"x,y"}}, {3, {"y"}}});
    REQUIRE(usesStore.getVariablesUsedBy(1) == std::unordered_set<variable> {"x"});
    REQUIRE(usesStore.getVariablesUsedBy(2) == std::unordered_set<variable> {"x", "y"});
    REQUIRE(usesStore.getVariablesUsedBy(3) == std::unordered_set<variable> {"y"});
}

TEST_CASE("Test Uses store getStatementsUsing") {
    auto usesStore = UsesStore();
    usesStore.addLineUsesVar(std::unordered_map<statementNumber, std::unordered_set<variable>>
    {{1, {"x"}}, {2, {"x","y"}}, {3, {"y"}}});
    std::unordered_set<statementNumber> statements = usesStore.getStatementsUsing("x");
    REQUIRE(statements ==  std::unordered_set<statementNumber> {1,2});
    statements = usesStore.getStatementsUsing("y");
    REQUIRE(statements ==  std::unordered_set<statementNumber> {2,3});
}