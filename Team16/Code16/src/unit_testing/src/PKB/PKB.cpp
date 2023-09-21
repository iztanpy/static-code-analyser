#include "catch.hpp"
#include <stdio.h>
#include <unordered_set>
#include <iostream>
using namespace std;

#include "PKB/PKB.h"
#include "PKB/API/ReadFacade.h"
#include "PKB/API/WriteFacade.h"
#include "PKB/Helper/Wildcard.h"


TEST_CASE(" 1") {
	PKB pkb = PKB();
	auto writeFacade = WriteFacade(pkb);
	auto readFacade = ReadFacade(pkb);

	//make unordered map for <int, string > for assignments
	std::unordered_map<statementNumber, std::unordered_set<possibleCombinations>> assignmentsRHS;
    assignmentsRHS.insert({ 1, { "x", "y" } });
    assignmentsRHS.insert({ 2, { "y" } });
    assignmentsRHS.insert({ 3, { "x", "1", "2" } });

	std::unordered_map<statementNumber, variable> assignmentsLHS;
	assignmentsLHS.insert({ 1, "a" });
	assignmentsLHS.insert({ 2, "b" });
    assignmentsLHS.insert({ 3, "b" });
	
	writeFacade.storeAssignments(assignmentsRHS, assignmentsLHS);

    REQUIRE(readFacade.getAllAssigns().size() == 3);

	for (int value : readFacade.getAssigns(Wildcard(), "1")) {
        REQUIRE(value == 3);
	}

	for (int value : readFacade.getAssigns("b", "y")) {
		REQUIRE(value == 2);
	}

    REQUIRE(readFacade.getAssigns(Wildcard() , Wildcard()).size() == 3);

    REQUIRE(readFacade.getAssigns("b", Wildcard()).size() == 2);

	writeFacade.storeVariables({ "x", "y", "z", "a", "b" });

	for (std::string value : readFacade.getVariables()) {
		REQUIRE((value == "x" || value == "y" || value == "z" || value == "a" || value == "b"));
	}

	writeFacade.storeUses({{1, {"x", "y"}}, {2, {"y", "z"}} });

	for (std::string value : readFacade.uses(1)) {
		REQUIRE((value == "x" || value == "y"));
	}

	for (std::string value : readFacade.uses(2)) {
		REQUIRE((value == "y" || value == "z"));
	}
	writeFacade.storeConstants({ "1", "2", "3" });

	for (std::string value : readFacade.getConstants()) {
		REQUIRE((value == "1" || value == "2" || value == "3"));
	}
}


TEST_CASE("test Facades for AssignStore") {
    PKB pkb = PKB();
    WriteFacade writeFacade = WriteFacade(pkb);
    ReadFacade readFacade = ReadFacade(pkb);

    std::unordered_map<statementNumber, std::unordered_set<possibleCombinations>> assignmentsRHS;
    assignmentsRHS.insert({ 1, { "x", "y" } });
    assignmentsRHS.insert({ 2, { "x" } });
    assignmentsRHS.insert({ 3, { "x", "1", "2" } });

    std::unordered_map<statementNumber, variable> assignmentsLHS;
    assignmentsLHS.insert({ 1, "a" });
    assignmentsLHS.insert({ 2, "b" });
    assignmentsLHS.insert({ 3, "b" });

    writeFacade.storeAssignments(assignmentsRHS, assignmentsLHS);

    REQUIRE(readFacade.getAllAssigns().size() == 3);

    REQUIRE(readFacade.getAssigns(Wildcard(), Wildcard()).size() == 3);

    REQUIRE(readFacade.getAssigns("b", Wildcard()).size() == 2);

    REQUIRE(readFacade.getAssigns("b", "x").size() == 2);

    REQUIRE(readFacade.getAssigns("b", "1").size() == 1);

    REQUIRE(readFacade.getAssigns("b", "2").size() == 1);

    writeFacade.storeVariables({ "x", "y", "z", "a", "b" });

    for (std::string value : readFacade.getVariables()) {
        REQUIRE((value == "x" || value == "y" || value == "z" || value == "a" || value == "b"));
    }

    writeFacade.storeUses({ {1, {"x", "y"}}, {2, {"y", "z"}} });

    for (std::string value : readFacade.uses(1)) {
        REQUIRE((value == "x" || value == "y"));
    }

    for (std::string value : readFacade.uses(2)) {
        REQUIRE((value == "y" || value == "z"));
    }
}

TEST_CASE("Test Facades for Variable Store"){
    PKB pkb = PKB();
    WriteFacade writeFacade = WriteFacade(pkb);
    ReadFacade readFacade = ReadFacade(pkb);

    writeFacade.storeVariables({"x", "y", "z", "a", "b"});

    for (std::string value : readFacade.getVariables()) {
        REQUIRE((value == "x" || value == "y" || value == "z" || value == "a" || value == "b"));
    }
}

TEST_CASE("Test Facades for Uses Store") {
    PKB pkb = PKB();
    WriteFacade writeFacade = WriteFacade(pkb);
    ReadFacade readFacade = ReadFacade(pkb);

    writeFacade.storeUses({ {1, {"x", "y"}}, {2, {"y", "z"}} });

    for (std::string value : readFacade.uses(1)) {
        REQUIRE((value == "x" || value == "y"));
    }

    for (std::string value : readFacade.uses(2)) {
        REQUIRE((value == "y" || value == "z"));
    }
}

TEST_CASE("Test Parent Stores") {
    PKB pkb = PKB();

    pkb.storeParent({{1, {2, 3}}, {2, {4, 5}}, {3, {6, 7}}});

    Wildcard wildcard = Wildcard();

    REQUIRE(pkb.parent(wildcard, 2) == 1);
    REQUIRE(pkb.parent(wildcard, 3) == 1);
    REQUIRE(pkb.parent(wildcard, 4) == 2);
    REQUIRE(pkb.parent(wildcard, 5) == 2);
    REQUIRE(pkb.parent(wildcard, 6) == 3);
    REQUIRE(pkb.parent(wildcard, 7) == 3);

    for (auto value : pkb.parent(1, wildcard)) {
		REQUIRE((value == 2 || value == 3));
	}

    for (auto value : pkb.parent(2, wildcard)) {
        REQUIRE((value == 4 || value == 5));
    }

    for (auto value : pkb.parent(3, wildcard)) {
		REQUIRE((value == 6 || value == 7));
	}

    for (auto value : pkb.parentStar(1, wildcard)) {
		REQUIRE((value == 2 || value == 3 || value == 4 || value == 5 || value == 6 || value == 7));
	}

    for (auto value : pkb.parentStar(wildcard, 2)) {
        REQUIRE((value == 1));
    }

    for (auto value : pkb.parentStar(wildcard, 3)) {
		REQUIRE((value == 1));
	}

    for (auto value : pkb.parentStar(wildcard, 4)) {
		REQUIRE((value == 2 || value == 1));
	}

    for (auto value : pkb.parentStar(wildcard, 6)) {
        REQUIRE((value == 3 || value == 1));
    }

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

    REQUIRE(!pkb.isParent(wildcard, 1));
    REQUIRE(pkb.isParent(wildcard, 2));
    REQUIRE(pkb.isParent(wildcard, 3));
    REQUIRE(pkb.isParent(wildcard, 4));
    REQUIRE(pkb.isParent(wildcard, 5));
    REQUIRE(pkb.isParent(wildcard, 6));
    REQUIRE(pkb.isParent(wildcard, 7));

    REQUIRE(pkb.isParent(1, wildcard));
    REQUIRE(pkb.isParent(2, wildcard));
    REQUIRE(pkb.isParent(3, wildcard));
    REQUIRE(!pkb.isParent(4, wildcard));
    REQUIRE(!pkb.isParent(5, wildcard));
    REQUIRE(!pkb.isParent(6, wildcard));
    REQUIRE(!pkb.isParent(7, wildcard));

    REQUIRE(!pkb.isParentStar(wildcard, 1));
    REQUIRE(pkb.isParentStar(wildcard, 2));
    REQUIRE(pkb.isParentStar(wildcard, 3));
    REQUIRE(pkb.isParentStar(wildcard, 4));
    REQUIRE(pkb.isParentStar(wildcard, 5));
    REQUIRE(pkb.isParentStar(wildcard, 6));
    REQUIRE(pkb.isParentStar(wildcard, 7));
}

TEST_CASE("Test Uses stores") {
    PKB pkb = PKB();

    pkb.storeUses({ {1, {"x", "y"}}, {2, {"y", "z"}}, {3, {"z"}} });

    REQUIRE(pkb.isUses(1, "x"));
    REQUIRE(pkb.isUses(1, "y"));
    REQUIRE(pkb.isUses(2, "y"));
    REQUIRE(pkb.isUses(2, "z"));
    REQUIRE(!pkb.isUses(1, "z"));
    REQUIRE(!pkb.isUses(2, "x"));

    Wildcard wildcard = Wildcard();

    REQUIRE(pkb.isUses(1, wildcard));
    REQUIRE(pkb.isUses(2, wildcard));
    REQUIRE(pkb.isUses(3, wildcard));
    REQUIRE(!pkb.isUses(4, wildcard));

    // store StmtEntity
    pkb.addStatements({ { 1, StmtEntity::kAssign }, { 2, StmtEntity::kAssign }, { 3, StmtEntity::kRead } });
    
    variable x = "x";
    REQUIRE(pkb.uses(1) == std::unordered_set<variable>{"x", "y"});
    REQUIRE(pkb.uses(2) == std::unordered_set<variable>{"y", "z"});

    StmtEntity assign = StmtEntity::kAssign;
    REQUIRE(pkb.uses(assign, wildcard) == std::unordered_set<statementNumber>{1, 2});
    REQUIRE(pkb.uses(assign, x) == std::unordered_set<statementNumber>{1});
    REQUIRE(pkb.uses(assign, "y") == std::unordered_set<statementNumber>{1, 2});
    REQUIRE(pkb.uses(assign, "z") == std::unordered_set<statementNumber>{2});

    StmtEntity read = StmtEntity::kRead;
    REQUIRE(pkb.uses(read, wildcard) == std::unordered_set<statementNumber>{3});
    REQUIRE(pkb.uses(read, x) == std::unordered_set<statementNumber>{});
    REQUIRE(pkb.uses(read, "y") == std::unordered_set<statementNumber>{});
    REQUIRE(pkb.uses(read, "z") == std::unordered_set<statementNumber>{3});

    std::unordered_set<std::pair<statementNumber, variable>, PairHash> result = pkb.uses(assign);

    for (auto value : result) {
		REQUIRE((value.first == 1 || value.first == 2));
		REQUIRE((value.second == "x" || value.second == "y" || value.second == "z"));
	}

    result = pkb.uses(read);

    for (auto value : result) {
        REQUIRE((value.first == 3));
        REQUIRE((value.second == "z"));
    }
    
}

TEST_CASE("Test modifies stores") {
    PKB pkb = PKB();

    pkb.storeModifies({ {1, "x"}, {2, "y"}, {3, "x"} });

    REQUIRE(pkb.isModifies(1, "x"));
    REQUIRE(pkb.isModifies(2, "y"));
    REQUIRE(pkb.isModifies(3, "x"));
    REQUIRE(!pkb.isModifies(1, "y"));
    REQUIRE(!pkb.isModifies(2, "x"));

    Wildcard wildcard = Wildcard();

    REQUIRE(pkb.isModifies(1, wildcard));
    REQUIRE(pkb.isModifies(2, wildcard));
    REQUIRE(pkb.isModifies(3, wildcard));
    REQUIRE(!pkb.isModifies(4, wildcard));

    REQUIRE(pkb.modifies(1) == "x");
    REQUIRE(pkb.modifies(2) == "y");
    REQUIRE(pkb.modifies(3) == "x");

    // store StmtEntity
    pkb.addStatements({ { 1, StmtEntity::kAssign }, { 2, StmtEntity::kAssign }, { 3, StmtEntity::kRead } });

    REQUIRE(pkb.modifies(StmtEntity::kAssign, "x") == std::unordered_set<statementNumber>{1});
    REQUIRE(pkb.modifies(StmtEntity::kAssign, "y") == std::unordered_set<statementNumber>{2});
    REQUIRE(pkb.modifies(StmtEntity::kRead, "x") == std::unordered_set<statementNumber>{3});
    REQUIRE(pkb.modifies(StmtEntity::kRead, "y") == std::unordered_set<statementNumber>{});

    REQUIRE(pkb.modifies(StmtEntity::kAssign, wildcard) == std::unordered_set<statementNumber>{1, 2});
    REQUIRE(pkb.modifies(StmtEntity::kRead, wildcard) == std::unordered_set<statementNumber>{3});
    REQUIRE(pkb.modifies(StmtEntity::kIf, wildcard) == std::unordered_set<statementNumber>{});

    std::unordered_set<std::pair<statementNumber, variable>, PairHash> result = pkb.modifies(StmtEntity::kAssign);

    for (auto value : result) {
		REQUIRE((value.first == 1 || value.first == 2));
		REQUIRE((value.second == "x" || value.second == "y"));
	}

    result = pkb.modifies(StmtEntity::kRead);

    for (auto value : result) {
		REQUIRE((value.first == 3));
		REQUIRE((value.second == "x"));
	}

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
//    REQUIRE(readFacade.Uses(1) == std::unordered_set<std::string>{"a", "b"});
//}
