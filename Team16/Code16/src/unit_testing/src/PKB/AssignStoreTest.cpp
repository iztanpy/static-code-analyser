//
// Created by Isaac Tan on 17/9/23.
//

#include <unordered_set>
#include "catch.hpp"
#include "PKB/Stores/AssignStore.h"

TEST_CASE("Test Assign store add") {
    auto assignStore = AssignStore();
    assignStore.addNumRHSMap({{1, {"x"}}, {2, {"x + y", "x", "y"}}, {3, {"y"}}});
    assignStore.addNumLHSMap({{1, "x"}, {2, "y"}, {3, "x"}});
    REQUIRE(assignStore.getAllAssigns().size() == 3);
}

