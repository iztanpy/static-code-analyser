#include <stdio.h>
#include <unordered_set>
#include "catch.hpp"

#include "PKB/PKB.h"
#include "PKB/API/ReadFacade.h"
#include "PKB/API/WriteFacade.h"


TEST_CASE(" 1") {
	PKB pkb = PKB();
	auto writeFacade = WriteFacade(pkb);
	auto readFacade = ReadFacade(pkb);

	//make unordered map for <int, string > for assignments
	std::unordered_map<statementNumber, std::unordered_set<possibleCombinations>> assignmentsRHS;
	assignmentsRHS.insert({ 1, { "x + y", "y + a" } });
	assignmentsRHS.insert({ 2, { "x + y", "y + z" } });
    assignmentsRHS.insert({ 3, { "y + w", "x" } });

	std::unordered_map<statementNumber, variable> assignmentsLHS;
	assignmentsLHS.insert({ 1, "a" });
	assignmentsLHS.insert({ 2, "b" });
    assignmentsLHS.insert({ 3, "b" });
	
	writeFacade.storeAssignments(assignmentsRHS, assignmentsLHS);

    REQUIRE(readFacade.getAllAssigns().size() == 3);

	for (int value : readFacade.getAssigns("a", "x + y")) {
		REQUIRE(value == 1);
	}

	for (int value : readFacade.getAssigns("b", "y + z")) {
		REQUIRE(value == 2);
	}

    REQUIRE(readFacade.getAssigns("_", "x + y").size() == 2);

    REQUIRE(readFacade.getAssigns("_", "_").size() == 3);

    REQUIRE(readFacade.getAssigns("b", "_").size() == 2);

	writeFacade.storeVariables({ "x", "y", "z", "a", "b" });

	for (std::string value : readFacade.getAllVariables()) {
		REQUIRE((value == "x" || value == "y" || value == "z" || value == "a" || value == "b"));
	}

	writeFacade.storeUses({{1, {"x", "y"}}, {2, {"y", "z"}} });

	for (std::string value : readFacade.getVariablesUsedBy(1)) {
		REQUIRE((value == "x" || value == "y"));
	}

	for (std::string value : readFacade.getVariablesUsedBy(2)) {
		REQUIRE((value == "y" || value == "z"));
	}
	writeFacade.storeConstants({ "1", "2", "3" });

	for (std::string value : readFacade.getAllConstants()) {
		REQUIRE((value == "1" || value == "2" || value == "3"));
	}
}




TEST_CASE("test Facades for AssignStore") {
    PKB pkb = PKB();
    WriteFacade writeFacade = WriteFacade(pkb);
    ReadFacade readFacade = ReadFacade(pkb);

    std::unordered_map<statementNumber, std::unordered_set<possibleCombinations>> assignmentsRHS;
    assignmentsRHS.insert({ 1, { "x + y", "y + a" } });
    assignmentsRHS.insert({ 2, { "x + y", "y + z" } });
    assignmentsRHS.insert({ 3, { "y + w", "x" } });

    std::unordered_map<statementNumber, variable> assignmentsLHS;
    assignmentsLHS.insert({ 1, "a" });
    assignmentsLHS.insert({ 2, "b" });
    assignmentsLHS.insert({ 3, "b" });

    writeFacade.storeAssignments(assignmentsRHS, assignmentsLHS);

    REQUIRE(readFacade.getAllAssigns().size() == 3);

//    for (int value: readFacade.getAllAssigns()) {
//        std::cout << value << std::endl;
//    }

    for (int value : readFacade.getAssigns("a", "x + y")) {
        REQUIRE(value == 1);
    }

    for (int value : readFacade.getAssigns("b", "y + z")) {
        REQUIRE(value == 2);
    }

    REQUIRE(readFacade.getAssigns("_", "x + y").size() == 2);

    REQUIRE(readFacade.getAssigns("_", "_").size() == 3);

    REQUIRE(readFacade.getAssigns("b", "_").size() == 2);

    writeFacade.storeVariables({ "x", "y", "z", "a", "b" });

    for (std::string value : readFacade.getAllVariables()) {
        REQUIRE((value == "x" || value == "y" || value == "z" || value == "a" || value == "b"));
    }

    writeFacade.storeUses({ {1, {"x", "y"}}, {2, {"y", "z"}} });

    for (std::string value : readFacade.getVariablesUsedBy(1)) {
        REQUIRE((value == "x" || value == "y"));
    }

    for (std::string value : readFacade.getVariablesUsedBy(2)) {
        REQUIRE((value == "y" || value == "z"));
    }
}

TEST_CASE("Test Facades for Variable Store"){
    PKB pkb = PKB();
    WriteFacade writeFacade = WriteFacade(pkb);
    ReadFacade readFacade = ReadFacade(pkb);

    writeFacade.storeVariables({"x", "y", "z", "a", "b"});

    for (std::string value : readFacade.getAllVariables()) {
        REQUIRE((value == "x" || value == "y" || value == "z" || value == "a" || value == "b"));
    }
}

TEST_CASE("Test Facades for Uses Store") {
    PKB pkb = PKB();
    WriteFacade writeFacade = WriteFacade(pkb);
    ReadFacade readFacade = ReadFacade(pkb);

    writeFacade.storeUses({ {1, {"x", "y"}}, {2, {"y", "z"}} });

    for (std::string value : readFacade.getVariablesUsedBy(1)) {
        REQUIRE((value == "x" || value == "y"));
    }

    for (std::string value : readFacade.getVariablesUsedBy(2)) {
        REQUIRE((value == "y" || value == "z"));
    }
}

TEST_CASE("Test Parent Stores") {
    PKB pkb = PKB();

    pkb.storeParent({{1, {2, 3}}, {2, {4, 5}}, {3, {6, 7}}});

    REQUIRE(pkb.getParent(2) == 1);
    REQUIRE(pkb.getParent(3) == 1);
    REQUIRE(pkb.getParent(4) == 2);

    REQUIRE(pkb.isParent(1, 2));
    REQUIRE(pkb.isParent(1, 3));
    REQUIRE(pkb.isParent(2, 4));
    REQUIRE(pkb.isParent(2, 5));
    REQUIRE(pkb.isParent(3, 6));
    REQUIRE(pkb.isParent(3, 7));
    REQUIRE(!pkb.isParent(1, 4));
    REQUIRE(!pkb.isParent(1, 5));
    REQUIRE(!pkb.isParent(1, 6));
    REQUIRE(!pkb.isParent(1, 7));
    REQUIRE(!pkb.isParent(2, 6));

    REQUIRE(pkb.isParentStar(1, 2));
    REQUIRE(pkb.isParentStar(1, 3));
    REQUIRE(pkb.isParentStar(1, 4));
    REQUIRE(pkb.isParentStar(1, 5));
    REQUIRE(pkb.isParentStar(1, 6));
    REQUIRE(pkb.isParentStar(1, 7));
    REQUIRE(pkb.isParentStar(2, 4));
    REQUIRE(pkb.isParentStar(2, 5));
    REQUIRE(pkb.isParentStar(3, 6));
    REQUIRE(pkb.isParentStar(3, 7));
    REQUIRE(!pkb.isParentStar(1, 1));
    REQUIRE(!pkb.isParentStar(2, 2));
    REQUIRE(!pkb.isParentStar(3, 1));
    REQUIRE(!pkb.isParentStar(3, 2));
}


//TEST_CASE("Test PKB") {
//    std::unordered_set<int> assignments = {1, 2, 3};
//    std::unordered_set<std::string> variables = {"a", "b", "c"};
//    std::unordered_set<std::string> constants = {"1", "2", "3"};
//    std::unordered_map<std::string, std::unordered_set<std::string>> UsesConst;
//    std::unordered_map<std::string, std::unordered_set<std::string>> UsesVar;
//    std::unordered_map<int, std::unordered_set<std::string>> LineUses;
//    PKB pkb = PKB(assignments, variables, constants, UsesConst, UsesVar, LineUses);
//    std::unordered_set<int> assignments_out = pkb.getAssignments();
//    std::unordered_set<std::string> variables_out = pkb.getVariables();
//    std::unordered_set<std::string> constants_out = pkb.getConstants();
//
//    REQUIRE(assignments_out.size() == assignments.size());
//    REQUIRE(variables_out.size() == variables.size());
//    REQUIRE(constants_out.size() == constants.size());
//    for (const auto& value : assignments) {
//        REQUIRE(assignments_out.find(value) != assignments_out.end());
//    }
//    for (const auto& value : variables) {
//        REQUIRE(variables_out.find(value) != variables_out.end());
//    }
//    for (const auto& value : constants) {
//        REQUIRE(constants_out.find(value) != constants_out.end());
//    }
//}
//
//TEST_CASE("Test Read and Write Facades") {
//    std::unordered_set<int> assignments = {1, 2, 3};  // Change vector to unordered_set
//    std::unordered_set<std::string> variables = {"a", "b", "c"};  // Change vector to unordered_set
//    std::unordered_set<std::string> constants = {"1", "2", "3"};  // Change vector to unordered_set
//    std::unordered_map<std::string, std::unordered_set<std::string>> UsesConst;
//    std::unordered_map<std::string, std::unordered_set<std::string>> UsesVar;
//    std::unordered_map<int, std::unordered_set<std::string>> LineUses;
//    PKB pkb = PKB(assignments, variables, constants, UsesConst, UsesVar, LineUses);
//
//    WriteFacade writeFacade = WriteFacade(&pkb);
//
//    ReadFacade readFacade = ReadFacade(&pkb);
//
//    for (const auto& value : assignments) {
//        REQUIRE(assignments.find(value) != assignments.end());
//    }
//
//    for (const auto& value : variables) {
//        REQUIRE(variables.find(value) != variables.end());
//    }
//
//    for (const auto& value : constants) {
//        REQUIRE(constants.find(value) != constants.end());
//    }
//
//    writeFacade.storeAssignments({4, 5, 6});
//
//    writeFacade.storeVariables({"d", "e", "f"});
//
//    writeFacade.storeConstants({"4", "5", "6"});
//
//    writeFacade.storeLineUses({{1, {"a", "b"}}, {2, {"c", "d"}}, {3, {"e", "f"}}});
//
//    writeFacade.storeUsesConst({{"a", {"1", "2"}}, {"b", {"3", "4"}}});
//
//    writeFacade.storeUsesVar({{"a", {"b", "c"}}, {"d", {"e", "f"}}});
//
//    for (const auto& value : assignments) {
//        REQUIRE(assignments.find(value) != assignments.end());
//    }
//
//    for (const auto& value : variables) {
//        REQUIRE(variables.find(value) != variables.end());
//    }
//
//    for (const auto& value : constants) {
//        REQUIRE(constants.find(value) != constants.end());
//    }
//
//    REQUIRE(readFacade.getVariablesUsedBy(1) == std::unordered_set<std::string>{"a", "b"});
//}
