#include "catch.hpp"
#include <unordered_set>
#include <iostream>
using namespace std;

#include "PKB/pkb.h"
#include "PKB/API/read_facade.h"
#include "PKB/API/write_facade.h"
#include "utils/entity_types.h"

TEST_CASE(" 1") {
  PKB pkb = PKB();
  auto writeFacade = write_facade(pkb);
  auto readFacade = ReadFacade(pkb);

  //make unordered map for <int, string > for assignments
  std::unordered_map < statementNumber, std::unordered_set < possibleCombinations >> assignmentsRHS;
  assignmentsRHS.insert({1, {"x", "y"}});
  assignmentsRHS.insert({2, {"y"}});
  assignmentsRHS.insert({3, {"x", "1", "2"}});

  std::unordered_map < statementNumber, variable > assignmentsLHS;
  assignmentsLHS.insert({1, "a"});
  assignmentsLHS.insert({2, "b"});
  assignmentsLHS.insert({3, "b"});

  writeFacade.storeAssignments(assignmentsRHS, assignmentsLHS);

  REQUIRE(readFacade.getAllAssigns().size() == 3);

  for (int value : readFacade.getAssigns(Wildcard(), ExactExpr{"1"})) {
    REQUIRE(value == 3);
  }

  for (int value : readFacade.getAssigns("b", ExactExpr{"y"})) {
    REQUIRE(value == 2);
  }

  REQUIRE(readFacade.getAssigns(Wildcard(), Wildcard()).size() == 3);

  REQUIRE(readFacade.getAssigns("b", Wildcard()).size() == 2);

  writeFacade.storeVariables({"x", "y", "z", "a", "b"});

  for (std::string value : readFacade.getVariables()) {
    REQUIRE((value == "x" || value == "y" || value == "z" || value == "a" || value == "b"));
  }

  writeFacade.storeUses({{1, {"x", "y"}}, {2, {"y", "z"}}});

  for (std::string value : readFacade.uses(1)) {
    REQUIRE((value == "x" || value == "y"));
  }

  for (std::string value : readFacade.uses(2)) {
    REQUIRE((value == "y" || value == "z"));
  }
  writeFacade.storeConstants({"1", "2", "3"});

  for (std::string value : readFacade.getConstants()) {
    REQUIRE((value == "1" || value == "2" || value == "3"));
  }
}

TEST_CASE("test Facades for assign_store") {
  PKB pkb = PKB();
  write_facade writeFacade = write_facade(pkb);
  ReadFacade readFacade = ReadFacade(pkb);

  std::unordered_map < statementNumber, std::unordered_set < possibleCombinations >> assignmentsRHS;
  assignmentsRHS.insert({1, {"x", "y"}});
  assignmentsRHS.insert({2, {"x"}});
  assignmentsRHS.insert({3, {"x", "1", "2"}});

  std::unordered_map < statementNumber, variable > assignmentsLHS;
  assignmentsLHS.insert({1, "a"});
  assignmentsLHS.insert({2, "b"});
  assignmentsLHS.insert({3, "b"});

  writeFacade.storeAssignments(assignmentsRHS, assignmentsLHS);

  REQUIRE(readFacade.getAllAssigns().size() == 3);

  REQUIRE(readFacade.getAssigns(Wildcard(), Wildcard()).size() == 3);

  REQUIRE(readFacade.getAssigns("b", Wildcard()).size() == 2);

  REQUIRE(readFacade.getAssigns("b", PartialExpr{"x"}).size() == 2);

  REQUIRE(readFacade.getAssigns("b", PartialExpr{"1"}).size() == 1);

  REQUIRE(readFacade.getAssigns("b", PartialExpr{"2"}).size() == 1);

  writeFacade.storeVariables({"x", "y", "z", "a", "b"});

  for (std::string value : readFacade.getVariables()) {
    REQUIRE((value == "x" || value == "y" || value == "z" || value == "a" || value == "b"));
  }

  writeFacade.storeUses({{1, {"x", "y"}}, {2, {"y", "z"}}});

  for (std::string value : readFacade.uses(1)) {
    REQUIRE((value == "x" || value == "y"));
  }

  for (std::string value : readFacade.uses(2)) {
    REQUIRE((value == "y" || value == "z"));
  }
}

TEST_CASE("Test Facades for Variable Store") {
  PKB pkb = PKB();
  write_facade writeFacade = write_facade(pkb);
  ReadFacade readFacade = ReadFacade(pkb);

  writeFacade.storeVariables({"x", "y", "z", "a", "b"});

  for (std::string value : readFacade.getVariables()) {
    REQUIRE((value == "x" || value == "y" || value == "z" || value == "a" || value == "b"));
  }
}

TEST_CASE("Test Facades for Uses Store") {
  PKB pkb = PKB();
  write_facade writeFacade = write_facade(pkb);
  ReadFacade readFacade = ReadFacade(pkb);

  writeFacade.storeUses({{1, {"x", "y"}}, {2, {"y", "z"}}});

  for (std::string value : readFacade.uses(1)) {
    REQUIRE((value == "x" || value == "y"));
  }

  for (std::string value : readFacade.uses(2)) {
    REQUIRE((value == "y" || value == "z"));
  }
}

TEST_CASE("Test Parent Stores 2") {
  PKB pkb = PKB();

  pkb.storeParent({{1, {2, 3}}, {2, {4, 5}}, {3, {6, 7}}});
  pkb.addStatements({{1, StmtEntity::kIf},
                     {2, StmtEntity::kIf},
                     {3, StmtEntity::kIf},
                     {4, StmtEntity::kAssign},
                     {5, StmtEntity::kAssign},
                     {6, StmtEntity::kAssign},
                     {7, StmtEntity::kRead}});

  Wildcard wildcard = Wildcard();

  // Test Wildcard wildcard, StmtEntity entity
  REQUIRE(pkb.parent(wildcard, StmtEntity::kAssign).size() == 3);
  REQUIRE(pkb.parent(wildcard, StmtEntity::kRead).size() == 1);


  // Test StmtEntity entity, statementNumber num
  REQUIRE(pkb.parent(StmtEntity::kIf, 4) == std::unordered_set < statementNumber > {2});
  REQUIRE(pkb.parent(StmtEntity::kRead, 6) == std::unordered_set < statementNumber > {});

  // Test StmtEntity entity, Wildcard wildcard
  REQUIRE(pkb.parent(StmtEntity::kIf, wildcard).size() == 3);
  REQUIRE(pkb.parent(StmtEntity::kAssign, wildcard).size() == 0);
  REQUIRE(pkb.parent(StmtEntity::kRead, wildcard).size() == 0);

  // Test StmtEntity entity, entity2
  REQUIRE(pkb.parent(StmtEntity::kIf, StmtEntity::kAssign).size() == 3);
  REQUIRE(pkb.parent(StmtEntity::kAssign, StmtEntity::kRead).size() == 0);

  // Test Wildcard wildcard, StmtEntity entity
  REQUIRE(pkb.parent(wildcard, StmtEntity::kAssign).size() == 3);
  REQUIRE(pkb.parent(wildcard, StmtEntity::kRead).size() == 1);

  // Test statementNumber num, StmtEntity entity
  REQUIRE(pkb.parent(1, StmtEntity::kAssign) == std::unordered_set < statementNumber > {});
  REQUIRE(pkb.parent(2, StmtEntity::kAssign) == std::unordered_set < statementNumber > {4, 5});
  REQUIRE(pkb.parent(3, StmtEntity::kRead) == std::unordered_set < statementNumber > {7});
  REQUIRE(pkb.parent(3, StmtEntity::kAssign) == std::unordered_set < statementNumber > {6});

  // PARENTS STAR

  // Test StmtEntity num
  REQUIRE(pkb.parentStar(StmtEntity::kIf, 2) == std::unordered_set < statementNumber > {1});
  REQUIRE(pkb.parentStar(StmtEntity::kIf, 4) == std::unordered_set < statementNumber > {1, 2});
  REQUIRE(pkb.parentStar(StmtEntity::kAssign, 6) == std::unordered_set < statementNumber > {});


  // Test StmtEntity entity, Wildcard wildcard
  REQUIRE(pkb.parentStar(StmtEntity::kIf, wildcard).size() == 3);
  REQUIRE(pkb.parentStar(StmtEntity::kAssign, wildcard).size() == 0);
  REQUIRE(pkb.parentStar(StmtEntity::kRead, wildcard).size() == 0);

  // Test StmtEntity entity, entity2
  REQUIRE(pkb.parentStar(StmtEntity::kIf, StmtEntity::kAssign).size() == 6);
  REQUIRE(pkb.parentStar(StmtEntity::kAssign, StmtEntity::kRead).size() == 0);

  // Test Wildcard wildcard, StmtEntity entity
  REQUIRE(pkb.parentStar(wildcard, StmtEntity::kAssign).size() == 3);
  REQUIRE(pkb.parentStar(wildcard, StmtEntity::kRead).size() == 1);

  // Test statementNumber num, StmtEntity entity
  REQUIRE(pkb.parentStar(1, StmtEntity::kAssign) == std::unordered_set < statementNumber > {4, 5, 6});
  REQUIRE(pkb.parentStar(2, StmtEntity::kAssign) == std::unordered_set < statementNumber > {4, 5});
  REQUIRE(pkb.parentStar(3, StmtEntity::kRead) == std::unordered_set < statementNumber > {7});
  REQUIRE(pkb.parentStar(3, StmtEntity::kAssign) == std::unordered_set < statementNumber > {6});

  REQUIRE(pkb.parent(wildcard, 2) == std::unordered_set < statementNumber > {1});
  REQUIRE(pkb.parent(wildcard, 3) == std::unordered_set < statementNumber > {1});
  REQUIRE(pkb.parent(wildcard, 4) == std::unordered_set < statementNumber > {2});
  REQUIRE(pkb.parent(wildcard, 5) == std::unordered_set < statementNumber > {2});
  REQUIRE(pkb.parent(wildcard, 6) == std::unordered_set < statementNumber > {3});
  REQUIRE(pkb.parent(wildcard, 7) == std::unordered_set < statementNumber > {3});

  for (auto value : pkb.parent(1, wildcard)) {
    REQUIRE((value == 2 || value == 3));
  }

  for (auto value : pkb.parent(2, wildcard)) {
    REQUIRE((value == 4 || value == 5));
  }

  for (auto value : pkb.parent(3, wildcard)) {
    REQUIRE((value == 6 || value == 7));
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

  pkb.storeUses({{1, {"x", "y"}}, {2, {"y", "z"}}, {3, {"z"}}});

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
  pkb.addStatements({{1, StmtEntity::kAssign}, {2, StmtEntity::kAssign}, {3, StmtEntity::kRead}});

  variable x = "x";
  REQUIRE(pkb.uses(1) == std::unordered_set < variable > {"x", "y"});
  REQUIRE(pkb.uses(2) == std::unordered_set < variable > {"y", "z"});

  StmtEntity assign = StmtEntity::kAssign;
  REQUIRE(pkb.uses(assign, wildcard) == std::unordered_set < statementNumber > {1, 2});
  REQUIRE(pkb.uses(assign, x) == std::unordered_set < statementNumber > {1});
  REQUIRE(pkb.uses(assign, "y") == std::unordered_set < statementNumber > {1, 2});
  REQUIRE(pkb.uses(assign, "z") == std::unordered_set < statementNumber > {2});

  StmtEntity read = StmtEntity::kRead;
  REQUIRE(pkb.uses(read, wildcard) == std::unordered_set < statementNumber > {3});
  REQUIRE(pkb.uses(read, x) == std::unordered_set < statementNumber > {});
  REQUIRE(pkb.uses(read, "y") == std::unordered_set < statementNumber > {});
  REQUIRE(pkb.uses(read, "z") == std::unordered_set < statementNumber > {3});

  std::unordered_set < std::pair<statementNumber, variable>, PairHash > result = pkb.uses(assign);

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

  pkb.storeParent({{1, {2, 3}}});

  pkb.storeModifies({{1, "x"}, {2, "y"}, {3, "x"}});

  REQUIRE(pkb.isModifies(1, "x"));
  REQUIRE(pkb.isModifies(2, "y"));
  REQUIRE(pkb.isModifies(3, "x"));
  REQUIRE(pkb.isModifies(1, "y"));
  REQUIRE(!pkb.isModifies(2, "x"));

  Wildcard wildcard = Wildcard();

  REQUIRE(pkb.isModifies(1, wildcard));
  REQUIRE(pkb.isModifies(2, wildcard));
  REQUIRE(pkb.isModifies(3, wildcard));
  REQUIRE(!pkb.isModifies(4, wildcard));

  // store StmtEntity
  pkb.addStatements({{1, StmtEntity::kAssign},
                     {2, StmtEntity::kAssign},
                     {3, StmtEntity::kRead}});

  REQUIRE(pkb.modifies(StmtEntity::kAssign, "x") == std::unordered_set < statementNumber > {1});
  REQUIRE(pkb.modifies(StmtEntity::kAssign, "y") == std::unordered_set < statementNumber > {2, 1});
  REQUIRE(pkb.modifies(StmtEntity::kRead, "x") == std::unordered_set < statementNumber > {3});
  REQUIRE(pkb.modifies(StmtEntity::kRead, "y") == std::unordered_set < statementNumber > {});

  REQUIRE(pkb.modifies(StmtEntity::kAssign, wildcard) == std::unordered_set < statementNumber > {1, 2});
  REQUIRE(pkb.modifies(StmtEntity::kRead, wildcard) == std::unordered_set < statementNumber > {3});
  REQUIRE(pkb.modifies(StmtEntity::kIf, wildcard) == std::unordered_set < statementNumber > {});

  std::unordered_set < std::pair<statementNumber, variable>, PairHash > result = pkb.modifies(StmtEntity::kAssign);

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

TEST_CASE("Tes follows store") {
  PKB pkb = PKB();

  pkb.storeFollows({{1, 2}, {2, 3}, {3, 4}});
  pkb.addStatements({{1, StmtEntity::kAssign}, {2, StmtEntity::kAssign}, {3, StmtEntity::kRead},
                     {4, StmtEntity::kAssign}});

  REQUIRE(pkb.isFollow(1, 2));
  REQUIRE(pkb.isFollow(2, 3));
  REQUIRE(pkb.isFollow(3, 4));

  REQUIRE(!pkb.isFollow(1, 3));
  REQUIRE(!pkb.isFollow(2, 4));
  REQUIRE(!pkb.isFollow(1, 4));

  REQUIRE(pkb.follows(StmtEntity::kAssign, 1) == std::unordered_set < statementNumber > {});
  REQUIRE(pkb.follows(StmtEntity::kAssign, 2) == std::unordered_set < statementNumber > {1});
  REQUIRE(pkb.follows(StmtEntity::kAssign, 3) == std::unordered_set < statementNumber > {2});
  REQUIRE(pkb.follows(StmtEntity::kRead, 4) == std::unordered_set < statementNumber > {3});

  REQUIRE(pkb.follows(StmtEntity::kAssign, Wildcard()) == std::unordered_set < statementNumber > {1, 2});
  REQUIRE(pkb.follows(StmtEntity::kRead, Wildcard()) == std::unordered_set < statementNumber > {3});

  std::unordered_set < std::pair<statementNumber, statementNumber>, PairHash >
      result = pkb.follows(StmtEntity::kAssign, StmtEntity::kAssign);

  for (auto value : result) {
    REQUIRE((value.first == 1));
    REQUIRE((value.second == 2));
  }

  result = pkb.follows(StmtEntity::kAssign, StmtEntity::kRead);
  for (auto value : result) {
    REQUIRE((value.first == 2));
    REQUIRE((value.second == 3));
  }

  result = pkb.followStar(StmtEntity::kAssign, StmtEntity::kAssign);

  for (auto value : result) {
    REQUIRE((value.first == 1 || value.first == 2));
    REQUIRE((value.second == 2 || value.second == 4));
  }
}