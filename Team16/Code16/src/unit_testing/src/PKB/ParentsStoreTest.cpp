//
// Created by Isaac Tan on 17/9/23.
//

#include <unordered_set>
#include "catch.hpp"
#include "PKB/Stores/ParentStore.h"

typedef int statementNumber;

TEST_CASE("Test Parent Store Add") {
    auto parentStore = ParentStore();
    parentStore.storeParent({{1, {2, 3}}, {2, {4, 5}}, {3, {6, 7}}});
    REQUIRE(parentStore.getParent(2) == 1);
    REQUIRE(parentStore.getParent(3) == 1);
    REQUIRE(parentStore.getParent(4) == 2);
    REQUIRE(parentStore.getParent(5) == 2);
    REQUIRE(parentStore.getParent(6) == 3);
    REQUIRE(parentStore.getParent(7) == 3);
}


TEST_CASE("Test getChildrens") {
    auto parentStore = ParentStore();
    parentStore.storeParent({{1, {2, 3}}, {2, {4, 5}}, {3, {6, 7}}});
    std::unordered_set<statementNumber> childrens = parentStore.getChildrens(1);
    REQUIRE(childrens ==  std::unordered_set<statementNumber> {2,3,4,5,6,7});
    childrens = parentStore.getChildrens(2);
    REQUIRE(childrens ==  std::unordered_set<statementNumber> {4,5});
    childrens = parentStore.getChildrens(3);
    REQUIRE(childrens ==  std::unordered_set<statementNumber> {6,7});

}
TEST_CASE("Test getParents") {
    auto parentStore = ParentStore();
    parentStore.storeParent({{1, {2, 3}}, {2, {4, 5}}, {3, {6, 7}}});
    std::unordered_set<statementNumber> parents = parentStore.getParents(2);
    REQUIRE(parents ==  std::unordered_set<statementNumber> {1});
    parents = parentStore.getParents(4);
    REQUIRE(parents ==  std::unordered_set<statementNumber> {1,2});
}

TEST_CASE("Test getChildren") {
    auto parentStore = ParentStore();
    parentStore.storeParent({{1, {2, 3}}, {2, {4, 5}}, {3, {6, 7}}});
    std::unordered_set<statementNumber> children = parentStore.getChildren(1);
    REQUIRE(children ==  std::unordered_set<statementNumber> {2,3});
    children = parentStore.getChildren(2);
    REQUIRE(children ==  std::unordered_set<statementNumber> {4,5});
    children = parentStore.getChildren(3);
    REQUIRE(children ==  std::unordered_set<statementNumber> {6,7});
}