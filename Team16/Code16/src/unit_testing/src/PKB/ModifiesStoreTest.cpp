#include <unordered_set>
#include <iostream>
#include "catch.hpp"
#include "PKB/Stores/RelationStore.h"

typedef int statementNumber;
typedef std::string variable;

TEST_CASE("Test Modifies Store Add") {
    auto modifiesStore = RelationStore();
    modifiesStore.storeRelation({{1, {"x"}}, {2, {"x", "y"}}, {3, {"x"}}});
    REQUIRE(modifiesStore.relates(1) == std::unordered_set<variable> {"x"});
    REQUIRE(modifiesStore.relates(2) == std::unordered_set<variable> {"x", "y"});
    REQUIRE(modifiesStore.relates(3) == std::unordered_set<variable> {"x"});
}

TEST_CASE("Test getStatementThatModifies") {
    auto modifiesStore = RelationStore();
    modifiesStore.storeRelation({{1, {"x"}}, {2, {"y"}}, {3, {"x"}}});
    std::unordered_set<statementNumber> statements = modifiesStore.relates("x");
    REQUIRE(statements ==  std::unordered_set<statementNumber> {1,3});
    statements = modifiesStore.relates("y");
    REQUIRE(statements ==  std::unordered_set<statementNumber> {2});

    REQUIRE(modifiesStore.isRelation(1, "x"));
    REQUIRE(modifiesStore.isRelation(2, "y"));
    REQUIRE(modifiesStore.isRelation(3, "x"));
    REQUIRE(!modifiesStore.isRelation(1, "y"));
    REQUIRE(!modifiesStore.isRelation(2, "x"));

    REQUIRE(modifiesStore.isRelation(1));
    REQUIRE(modifiesStore.isRelation(2));
    REQUIRE(modifiesStore.isRelation(3));
    REQUIRE(!modifiesStore.isRelation(4));
}

TEST_CASE("Test procedure methods") {
    auto modifiesStore = RelationStore();
    
    modifiesStore.storeRelation({ {1, {"x"}}, {2, {"y"}}, {3, {"x"}}, {5, {"a", "b"}}, {6,{"c"}}, {8, {"m"}}, {9, {"n"}} });

    std::unordered_map<std::string, std::pair<int, int>> procedures = { {"First", {1, 3}}, {"Second", {5, 7}} , {"Third", {8, 9}} };
    std::unordered_map<std::string, std::unordered_set<std::string>> callTableStar = { {"First", {"Second", "Third"}}, {"Second", {"Third"}} };


    modifiesStore.storeRelationProcedures(procedures, callTableStar);

    REQUIRE((modifiesStore.isRelation("First")));
    REQUIRE((modifiesStore.isRelation("Second")));
    REQUIRE((modifiesStore.isRelation("Third")));
    REQUIRE((!modifiesStore.isRelation("Fourth")));

    REQUIRE((modifiesStore.relatesProcedureProc("First") == std::unordered_set<std::string>{"x", "y", "a", "b","c", "m", "n"}));
    REQUIRE((modifiesStore.relatesProcedureProc("Second") == std::unordered_set<std::string>{"a", "b", "c", "m", "n"}));
    REQUIRE((modifiesStore.relatesProcedureProc("Third") == std::unordered_set<std::string>{"m", "n"}));
    
    REQUIRE((modifiesStore.isRelation("First", "x")));
    REQUIRE((modifiesStore.isRelation("First", "y")));
    REQUIRE((modifiesStore.isRelation("First", "a")));
    REQUIRE((modifiesStore.isRelation("First", "b")));
    REQUIRE((modifiesStore.isRelation("First", "m")));
    REQUIRE((modifiesStore.isRelation("First", "n")));
    REQUIRE((modifiesStore.isRelation("First", "c")));

    REQUIRE((modifiesStore.isRelation("Second", "a")));
    REQUIRE((modifiesStore.isRelation("Second", "b")));
    REQUIRE((modifiesStore.isRelation("Second", "c")));
    REQUIRE((modifiesStore.isRelation("Second", "m")));
    REQUIRE((modifiesStore.isRelation("Second", "n")));
    REQUIRE((!modifiesStore.isRelation("Second", "x")));
    REQUIRE((!modifiesStore.isRelation("Second", "y")));

    REQUIRE((modifiesStore.isRelation("Third", "m")));
    REQUIRE((modifiesStore.isRelation("Third", "n")));
    REQUIRE((!modifiesStore.isRelation("Third", "x")));
    REQUIRE((!modifiesStore.isRelation("Third", "y")));
    REQUIRE((!modifiesStore.isRelation("Third", "a")));
    REQUIRE((!modifiesStore.isRelation("Third", "b")));
    REQUIRE((!modifiesStore.isRelation("Third", "c")));

    REQUIRE((modifiesStore.relatesProcedure() == std::unordered_set<std::string>{"First", "Second", "Third"}));
    
    REQUIRE((modifiesStore.relatesProcedure("x") == std::unordered_set<std::string>{"First"}));
    REQUIRE((modifiesStore.relatesProcedure("y") == std::unordered_set<std::string>{"First"}));
    REQUIRE((modifiesStore.relatesProcedure("a") == std::unordered_set<std::string>{"First", "Second"}));
    REQUIRE((modifiesStore.relatesProcedure("b") == std::unordered_set<std::string>{"First", "Second"}));
    REQUIRE((modifiesStore.relatesProcedure("c") == std::unordered_set<std::string>{"First", "Second"}));
    REQUIRE((modifiesStore.relatesProcedure("m") == std::unordered_set<std::string>{"First", "Second", "Third"}));
    REQUIRE((modifiesStore.relatesProcedure("n") == std::unordered_set<std::string>{"First", "Second", "Third"}));
    REQUIRE((modifiesStore.relatesProcedure("z") == std::unordered_set<std::string>{}));
}

TEST_CASE("Test getStatementThatModifies 2") {
    auto modifiesStore = RelationStore();
    modifiesStore.storeRelation({ {1, {"x"}}, {2, {"y"}}, {3, {}}, {4, {"x"}}, {5, {}}, {8, {"a"}} });
    modifiesStore.storeRelationProcedures({ {"First", {1, 4}}, {"Second", {5, 7}} , {"Third", {8, 9}} }, { {"First", {"Second", "Third"}}, {"Second", {}} });


    std::unordered_set<statementNumber> statements = modifiesStore.relates("x");
    REQUIRE(statements ==  std::unordered_set<statementNumber> {1,4});
    statements = modifiesStore.relates("y");
    REQUIRE(statements ==  std::unordered_set<statementNumber> {2});

    REQUIRE(modifiesStore.isRelation(1, "x"));
    REQUIRE(modifiesStore.isRelation(2, "y"));
    REQUIRE(!modifiesStore.isRelation(3, "x"));
    REQUIRE(!modifiesStore.isRelation(1, "y"));
    REQUIRE(!modifiesStore.isRelation(2, "x"));

    REQUIRE(modifiesStore.isRelation(1));
    REQUIRE(modifiesStore.isRelation(2));
    REQUIRE(!modifiesStore.isRelation(3));
    REQUIRE(modifiesStore.isRelation(4));
    REQUIRE(!modifiesStore.isRelation(5));
    REQUIRE(!modifiesStore.isRelation(6));

    REQUIRE((modifiesStore.relatesProcedureProc("First") == std::unordered_set<std::string>{"x", "y", "a"}));
    REQUIRE((modifiesStore.relatesProcedureProc("Second") == std::unordered_set<std::string>{}));
    REQUIRE((modifiesStore.relatesProcedureProc("Third") == std::unordered_set<std::string>{"a"}));

    REQUIRE((modifiesStore.isRelation("First", "x")));
    REQUIRE((modifiesStore.isRelation("First", "y")));
    REQUIRE((modifiesStore.isRelation("First", "a")));

    REQUIRE((!modifiesStore.isRelation("Second")));
    REQUIRE((!modifiesStore.isRelation("Fourth")));
    REQUIRE((modifiesStore.isRelation("First")));
    REQUIRE((modifiesStore.isRelation("Third")));

}

