#include <unordered_set>
#include "catch.hpp"
#include "PKB/Stores/AssignStore.h"
#include "utils/entity_types.h"

TEST_CASE("Test Assign store add") {
    auto assignStore = AssignStore();
    assignStore.addNumRHSMap({{1, {"x"}}, {2, {"x + y", "x", "y"}}, {3, {"y"}}});
    assignStore.addNumLHSMap({{1, "x"}, {2, "y"}, {3, "x"}});
    REQUIRE(assignStore.getAllAssigns().size() == 3);
}


TEST_CASE("Test") {
    auto assignStore = AssignStore();
    assignStore.addNumLHSMap({{1, "x"}, {2, "y"}, {3, "x"}});
    assignStore.storeFullPatternAssign({{1, "x"}, {2, "x + y"}, {3, "y"}});
    assignStore.storeAllPossibleCombinationsAssign({{1, {"x"}}, {2, {"x + y", "x", "y"}}, {3, {"y"}}});
    REQUIRE(assignStore.getAllAssigns().size() == 3);
    REQUIRE(assignStore.getAssignsFF("x", "y") == std::unordered_set<int>{3});
    REQUIRE(assignStore.getAssignsFF("x", "x") == std::unordered_set<int>{1});
    REQUIRE(assignStore.getAssignsWcF(Wildcard(), "y") == std::unordered_set<int>{3});
    REQUIRE(assignStore.getAssigns(Wildcard(), "x") == std::unordered_set<int>{1, 2});
    REQUIRE(assignStore.getAssigns(Wildcard(), "y") == std::unordered_set<int>{2, 3});
}
