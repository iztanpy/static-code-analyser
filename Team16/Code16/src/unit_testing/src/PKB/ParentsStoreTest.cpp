#include <unordered_set>
#include <iostream>
#include "catch.hpp"
#include "PKB/Stores/ParentStore.h"

typedef int statementNumber;

TEST_CASE("Test Parent Store Add") {
    auto parentStore = ParentStore();
    parentStore.storeParent({ {1, {2, 3}}, {2, {4, 5}}, {3, {6, 7}}, {6, {8, 9}} });
    REQUIRE(parentStore.getParent(2) == 1);
    REQUIRE(parentStore.getParent(3) == 1);
    REQUIRE(parentStore.getParent(4) == 2);
    REQUIRE(parentStore.getParent(5) == 2);
    REQUIRE(parentStore.getParent(6) == 3);
    REQUIRE(parentStore.getParent(7) == 3);

    REQUIRE(parentStore.isParent(1, 2));
    REQUIRE(parentStore.isParent(1, 3));
    REQUIRE(parentStore.isParent(2, 4));
    REQUIRE(parentStore.isParent(2, 5));
    REQUIRE(parentStore.isParent(3, 6));
    REQUIRE(parentStore.isParent(3, 7));
    REQUIRE(!parentStore.isParent(1, 4));
    REQUIRE(!parentStore.isParent(1, 5));
    REQUIRE(!parentStore.isParent(1, 6));
    REQUIRE(!parentStore.isParent(1, 7));
    REQUIRE(!parentStore.isParent(2, 6));

    REQUIRE(parentStore.isParentStar(1, 2));
    REQUIRE(parentStore.isParentStar(1, 3));
    REQUIRE(parentStore.isParentStar(1, 4));
    REQUIRE(parentStore.isParentStar(1, 5));
    REQUIRE(parentStore.isParentStar(1, 6));
    REQUIRE(parentStore.isParentStar(1, 7));
    REQUIRE(parentStore.isParentStar(2, 4));
    REQUIRE(parentStore.isParentStar(2, 5));
    REQUIRE(parentStore.isParentStar(3, 6));
    REQUIRE(parentStore.isParentStar(3, 7));
    REQUIRE(!parentStore.isParentStar(1, 1));
    REQUIRE(!parentStore.isParentStar(2, 2));
    REQUIRE(!parentStore.isParentStar(3, 1));
    REQUIRE(!parentStore.isParentStar(3, 2));


    Wildcard wildcard = Wildcard();

    REQUIRE(!parentStore.isParent(wildcard, 1));
    REQUIRE(parentStore.isParent(wildcard, 2));
    REQUIRE(parentStore.isParent(wildcard, 3));
    REQUIRE(parentStore.isParent(wildcard, 4));
    REQUIRE(parentStore.isParent(wildcard, 5));
    REQUIRE(parentStore.isParent(wildcard, 6));
    REQUIRE(parentStore.isParent(wildcard, 7));

    REQUIRE(parentStore.isParent(1, wildcard));
    REQUIRE(parentStore.isParent(2, wildcard));
    REQUIRE(parentStore.isParent(3, wildcard));
    REQUIRE(!parentStore.isParent(4, wildcard));
    REQUIRE(!parentStore.isParent(5, wildcard));
    REQUIRE(parentStore.isParent(6, wildcard));
    REQUIRE(!parentStore.isParent(7, wildcard));

    REQUIRE(!parentStore.isParentStar(wildcard, 1));
    REQUIRE(parentStore.isParentStar(wildcard, 2));
    REQUIRE(parentStore.isParentStar(wildcard, 3));
    REQUIRE(parentStore.isParentStar(wildcard, 4));
    REQUIRE(parentStore.isParentStar(wildcard, 5));
    REQUIRE(parentStore.isParentStar(wildcard, 6));
    REQUIRE(parentStore.isParentStar(wildcard, 7));

    REQUIRE(parentStore.isParentStar(1, wildcard));
    REQUIRE(parentStore.isParentStar(2, wildcard));
    REQUIRE(parentStore.isParentStar(3, wildcard));
    REQUIRE(!parentStore.isParentStar(4, wildcard));
    REQUIRE(!parentStore.isParentStar(5, wildcard));
    REQUIRE(parentStore.isParentStar(6, wildcard));
    REQUIRE(!parentStore.isParentStar(7, wildcard));

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