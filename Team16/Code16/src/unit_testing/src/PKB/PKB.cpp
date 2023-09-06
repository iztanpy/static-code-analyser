#include "catch.hpp"
#include <stdio.h>
#include "PKB/PKB.h"
#include "PKB/API/ReadFacade.h"
#include "PKB/API/WriteFacade.h"


TEST_CASE("Test PKB") {
     std::vector<int> assignments = { 1, 2, 3 };
     std::vector<std::string> variables = { "a", "b", "c" };
     std::vector<std::string> constants = { "1", "2", "3" };
     PKB pkb = PKB(assignments, variables, constants);
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

    PKB pkb = PKB(assignments, variables, constants);

    WriteFacade writeFacade = WriteFacade(&pkb);

    ReadFacade readFacade = readFacade = ReadFacade(&pkb);

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

    for (int i = 0; i < assignments.size(); i++) {
		REQUIRE(assignments[i] != readFacade.getAllAssigns()[i]);
	}

    for (int i = 0; i < variables.size(); i++) {
		REQUIRE(variables[i] != readFacade.getAllVariables()[i]);
	}

    for (int i = 0; i < constants.size(); i++) {
		REQUIRE(constants[i] != readFacade.getAllConstants()[i]);
	}
}