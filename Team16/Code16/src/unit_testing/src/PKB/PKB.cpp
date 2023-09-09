#include "catch.hpp"
#include <stdio.h>
#include "PKB/PKB.h"
#include "PKB/API/ReadFacade.h"
#include "PKB/API/WriteFacade.h"


TEST_CASE("Test PKB") {
     std::vector<int> assignments = { 1, 2, 3 };
     std::vector<std::string> variables = { "a", "b", "c" };
     std::vector<std::string> constants = { "1", "2", "3" };
     std::unordered_map<std::string, std::unordered_set<std::string>>  UsesConst;
     std::unordered_map<std::string, std::unordered_set<std::string>>  UsesVar;
     std::unordered_map<int, std::unordered_set<std::string>> LineUses;
     PKB pkb = PKB(assignments, variables, constants, UsesConst, UsesVar, LineUses);
     std::vector<int> assignments_out = pkb.getAssignments();
     std::vector<std::string> variables_out = pkb.getVariables();
     std::vector<std::string> constants_out = pkb.getConstants();


     REQUIRE(assignments_out.size() == assignments.size());
     REQUIRE(variables_out.size() == variables.size());
     REQUIRE(constants_out.size() == constants.size());
     for (int i = 0; i < assignments.size(); i++) {
         REQUIRE(assignments[i] == assignments_out[i]);
     }
     for (int i = 0; i < variables.size(); i++) {
         REQUIRE(variables[i] == variables_out[i]);
     }
     for (int i = 0; i < constants.size(); i++) {
         REQUIRE(constants[i] == constants_out[i]);
     }
}

TEST_CASE("Test Read and Write Facades") {
    std::vector<int> assignments = { 1, 2, 3 };
    std::vector<std::string> variables = { "a", "b", "c" };
    std::vector<std::string> constants = { "1", "2", "3" };
    std::unordered_map<std::string, std::unordered_set<std::string>>  UsesConst;
    std::unordered_map<std::string, std::unordered_set<std::string>>  UsesVar;
    std::unordered_map<int, std::unordered_set<std::string>> LineUses;
    PKB pkb = PKB(assignments, variables, constants, UsesConst, UsesVar, LineUses);

    WriteFacade writeFacade = WriteFacade(&pkb);

    ReadFacade readFacade = ReadFacade(&pkb);

    for (int i = 0; i < assignments.size(); i++) {
        REQUIRE(assignments[i] == readFacade.getAllAssigns()[i]);
    }

    for (int i = 0; i < variables.size(); i++) {
        REQUIRE(variables[i] == readFacade.getAllVariables()[i]);
    }

    for (int i = 0; i < constants.size(); i++) {
        REQUIRE(constants[i] == readFacade.getAllConstants()[i]);
    }

    writeFacade.storeAssignments({ 4, 5, 6 });

    writeFacade.storeVariables({ "d", "e", "f" });

    writeFacade.storeConstants({ "4", "5", "6" });

    writeFacade.storeLineUses({ {1, {"a", "b"}}, {2, {"c", "d"}}, {3, {"e", "f"}} });

    writeFacade.storeUsesConst({ {"a", {"1", "2"}}, {"b", {"3", "4"}} });

    writeFacade.storeUsesVar({ {"a", {"b", "c"}}, {"d", {"e", "f"}}});

    for (int i = 0; i < assignments.size(); i++) {
		REQUIRE(assignments[i] != readFacade.getAllAssigns()[i]);
	}

    for (int i = 0; i < variables.size(); i++) {
		REQUIRE(variables[i] != readFacade.getAllVariables()[i]);
	}

    for (int i = 0; i < constants.size(); i++) {
		REQUIRE(constants[i] != readFacade.getAllConstants()[i]);
	}

    REQUIRE(readFacade.getVariablesUsedBy(1) == std::unordered_set<std::string>{"a", "b"});
}