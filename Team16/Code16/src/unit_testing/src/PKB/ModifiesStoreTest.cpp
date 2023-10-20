#include <unordered_set>
#include <iostream>
#include "catch.hpp"
#include "PKB/Stores/RelationStore.h"

typedef int statementNumber;
typedef std::string variable;

TEST_CASE("Test Modifies Store Add") {
    auto modifiesStore = RelationStore();
    modifiesStore.storeModifies({{1, {"x"}}, {2, {"x", "y"}}, {3, {"x"}}});
    REQUIRE(modifiesStore.modifies(1) == std::unordered_set<variable> {"x"});
    REQUIRE(modifiesStore.modifies(2) == std::unordered_set<variable> {"x", "y"});
    REQUIRE(modifiesStore.modifies(3) == std::unordered_set<variable> {"x"});
}

TEST_CASE("Test getStatementThatModifies") {
    auto modifiesStore = RelationStore();
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

TEST_CASE("Test procedure methods") {
    auto modifiesStore = RelationStore();
    
    modifiesStore.storeModifies({ {1, {"x"}}, {2, {"y"}}, {3, {"x"}}, {5, {"a", "b"}}, {6,{"c"}}, {8, {"m"}}, {9, {"n"}} });

    std::unordered_map<std::string, std::pair<int, int>> procedures = { {"First", {1, 3}}, {"Second", {5, 7}} , {"Third", {8, 9}} };
    std::unordered_map<std::string, std::unordered_set<std::string>> callTableStar = { {"First", {"Second", "Third"}}, {"Second", {"Third"}} };


    modifiesStore.storeModifiesProcedures(procedures, callTableStar);

    REQUIRE((modifiesStore.isModifies("First")));
    REQUIRE((modifiesStore.isModifies("Second")));
    REQUIRE((modifiesStore.isModifies("Third")));
    REQUIRE((!modifiesStore.isModifies("Fourth")));

    REQUIRE((modifiesStore.modifiesProcedureProc("First") == std::unordered_set<std::string>{"x", "y", "a", "b","c", "m", "n"}));
    REQUIRE((modifiesStore.modifiesProcedureProc("Second") == std::unordered_set<std::string>{"a", "b", "c", "m", "n"}));
    REQUIRE((modifiesStore.modifiesProcedureProc("Third") == std::unordered_set<std::string>{"m", "n"}));
    
    REQUIRE((modifiesStore.isModifies("First", "x")));
    REQUIRE((modifiesStore.isModifies("First", "y")));
    REQUIRE((modifiesStore.isModifies("First", "a")));
    REQUIRE((modifiesStore.isModifies("First", "b")));
    REQUIRE((modifiesStore.isModifies("First", "m")));
    REQUIRE((modifiesStore.isModifies("First", "n")));
    REQUIRE((modifiesStore.isModifies("First", "c")));

    REQUIRE((modifiesStore.isModifies("Second", "a")));
    REQUIRE((modifiesStore.isModifies("Second", "b")));
    REQUIRE((modifiesStore.isModifies("Second", "c")));
    REQUIRE((modifiesStore.isModifies("Second", "m")));
    REQUIRE((modifiesStore.isModifies("Second", "n")));
    REQUIRE((!modifiesStore.isModifies("Second", "x")));
    REQUIRE((!modifiesStore.isModifies("Second", "y")));

    REQUIRE((modifiesStore.isModifies("Third", "m")));
    REQUIRE((modifiesStore.isModifies("Third", "n")));
    REQUIRE((!modifiesStore.isModifies("Third", "x")));
    REQUIRE((!modifiesStore.isModifies("Third", "y")));
    REQUIRE((!modifiesStore.isModifies("Third", "a")));
    REQUIRE((!modifiesStore.isModifies("Third", "b")));
    REQUIRE((!modifiesStore.isModifies("Third", "c")));

    REQUIRE((modifiesStore.modifiesProcedure() == std::unordered_set<std::string>{"First", "Second", "Third"}));
    
    REQUIRE((modifiesStore.modifiesProcedure("x") == std::unordered_set<std::string>{"First"}));
    REQUIRE((modifiesStore.modifiesProcedure("y") == std::unordered_set<std::string>{"First"}));
    REQUIRE((modifiesStore.modifiesProcedure("a") == std::unordered_set<std::string>{"First", "Second"}));
    REQUIRE((modifiesStore.modifiesProcedure("b") == std::unordered_set<std::string>{"First", "Second"}));
    REQUIRE((modifiesStore.modifiesProcedure("c") == std::unordered_set<std::string>{"First", "Second"}));
    REQUIRE((modifiesStore.modifiesProcedure("m") == std::unordered_set<std::string>{"First", "Second", "Third"}));
    REQUIRE((modifiesStore.modifiesProcedure("n") == std::unordered_set<std::string>{"First", "Second", "Third"}));
    REQUIRE((modifiesStore.modifiesProcedure("z") == std::unordered_set<std::string>{}));
}

