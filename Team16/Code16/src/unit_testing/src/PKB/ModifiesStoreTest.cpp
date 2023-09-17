#include <unordered_set>
#include "catch.hpp"
#include "PKB/Stores/ModifiesStore.h"

typedef int statementNumber;

TEST_CASE("Test Modifies Store Add") {
    auto modifiesStore = ModifiesStore();
    modifiesStore.storeModifies({{1, "x"}, {2, "y"}, {3, "x"}});
    REQUIRE(modifiesStore.getVariableModifiedBy(1) == "x");
    REQUIRE(modifiesStore.getVariableModifiedBy(2) == "y");
    REQUIRE(modifiesStore.getVariableModifiedBy(3) == "x");
}

TEST_CASE("Test getStatementThatModifies") {
    auto modifiesStore = ModifiesStore();
    modifiesStore.storeModifies({{1, "x"}, {2, "y"}, {3, "x"}});
    std::unordered_set<statementNumber> statements = modifiesStore.getStatementThatModifies("x");
    REQUIRE(statements ==  std::unordered_set<statementNumber> {1,3});
    statements = modifiesStore.getStatementThatModifies("y");
    REQUIRE(statements ==  std::unordered_set<statementNumber> {2});
}

