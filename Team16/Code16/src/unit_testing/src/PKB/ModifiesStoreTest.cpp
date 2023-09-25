#include <unordered_set>
#include "catch.hpp"
#include "PKB/Stores/ModifiesStore.h"

typedef int statementNumber;
typedef std::string variable;

TEST_CASE("Test Modifies Store Add") {
    auto modifiesStore = ModifiesStore();
    modifiesStore.storeModifies({{1, {"x"}}, {2, {"x", "y"}}, {3, {"x"}}});
    REQUIRE(modifiesStore.modifies(1) == std::unordered_set<variable> {"x"});
    REQUIRE(modifiesStore.modifies(2) == std::unordered_set<variable> {"x", "y"});
    REQUIRE(modifiesStore.modifies(3) == std::unordered_set<variable> {"x"});
}

TEST_CASE("Test getStatementThatModifies") {
    auto modifiesStore = ModifiesStore();
    modifiesStore.storeModifies({{1, {"x"}}, {2, {"y"}}, {3, {"x"}}});
    std::unordered_set<statementNumber> statements = modifiesStore.modifies("x");
    REQUIRE(statements ==  std::unordered_set<statementNumber> {1,3});
    statements = modifiesStore.modifies("y");
    REQUIRE(statements ==  std::unordered_set<statementNumber> {2});

    REQUIRE(modifiesStore.isModifies(1, "x"));
    REQUIRE(modifiesStore.isModifies(2, "y"));
    REQUIRE(modifiesStore.isModifies(3, "x"));
    REQUIRE(!modifiesStore.isModifies(1, "y"));
    REQUIRE(!modifiesStore.isModifies(2, "x"));

    REQUIRE(modifiesStore.isModifies(1));
    REQUIRE(modifiesStore.isModifies(2));
    REQUIRE(modifiesStore.isModifies(3));
    REQUIRE(!modifiesStore.isModifies(4));
}

