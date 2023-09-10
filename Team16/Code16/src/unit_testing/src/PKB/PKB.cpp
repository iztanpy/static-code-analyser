#include <stdio.h>
#include <unordered_set>
#include "catch.hpp"

#include "PKB/PKB.h"
#include "PKB/API/ReadFacade.h"
#include "PKB/API/WriteFacade.h"

TEST_CASE("Test PKB") {
    std::unordered_set<int> assignments = {1, 2, 3};
    std::unordered_set<std::string> variables = {"a", "b", "c"};
    std::unordered_set<std::string> constants = {"1", "2", "3"};
    std::unordered_map<std::string, std::unordered_set<std::string>> UsesConst;
    std::unordered_map<std::string, std::unordered_set<std::string>> UsesVar;
    std::unordered_map<int, std::unordered_set<std::string>> LineUses;
    PKB pkb = PKB(assignments, variables, constants, UsesConst, UsesVar, LineUses);
    std::unordered_set<int> assignments_out = pkb.getAssignments();
    std::unordered_set<std::string> variables_out = pkb.getVariables();
    std::unordered_set<std::string> constants_out = pkb.getConstants();

    REQUIRE(assignments_out.size() == assignments.size());
    REQUIRE(variables_out.size() == variables.size());
    REQUIRE(constants_out.size() == constants.size());
    for (const auto& value : assignments) {
        REQUIRE(assignments_out.find(value) != assignments_out.end());
    }
    for (const auto& value : variables) {
        REQUIRE(variables_out.find(value) != variables_out.end());
    }
    for (const auto& value : constants) {
        REQUIRE(constants_out.find(value) != constants_out.end());
    }
}

TEST_CASE("Test Read and Write Facades") {
    std::unordered_set<int> assignments = {1, 2, 3};  // Change vector to unordered_set
    std::unordered_set<std::string> variables = {"a", "b", "c"};  // Change vector to unordered_set
    std::unordered_set<std::string> constants = {"1", "2", "3"};  // Change vector to unordered_set
    std::unordered_map<std::string, std::unordered_set<std::string>> UsesConst;
    std::unordered_map<std::string, std::unordered_set<std::string>> UsesVar;
    std::unordered_map<int, std::unordered_set<std::string>> LineUses;
    PKB pkb = PKB(assignments, variables, constants, UsesConst, UsesVar, LineUses);

    WriteFacade writeFacade = WriteFacade(&pkb);

    ReadFacade readFacade = ReadFacade(&pkb);

    for (const auto& value : assignments) {
        REQUIRE(assignments.find(value) != assignments.end());
    }

    for (const auto& value : variables) {
        REQUIRE(variables.find(value) != variables.end());
    }

    for (const auto& value : constants) {
        REQUIRE(constants.find(value) != constants.end());
    }

    writeFacade.storeAssignments({4, 5, 6});

    writeFacade.storeVariables({"d", "e", "f"});

    writeFacade.storeConstants({"4", "5", "6"});

    writeFacade.storeLineUses({{1, {"a", "b"}}, {2, {"c", "d"}}, {3, {"e", "f"}}});

    writeFacade.storeUsesConst({{"a", {"1", "2"}}, {"b", {"3", "4"}}});

    writeFacade.storeUsesVar({{"a", {"b", "c"}}, {"d", {"e", "f"}}});

    for (const auto& value : assignments) {
        REQUIRE(assignments.find(value) != assignments.end());
    }

    for (const auto& value : variables) {
        REQUIRE(variables.find(value) != variables.end());
    }

    for (const auto& value : constants) {
        REQUIRE(constants.find(value) != constants.end());
    }

    REQUIRE(readFacade.getVariablesUsedBy(1) == std::unordered_set<std::string>{"a", "b"});
}
