#include "PKB/Stores/StatementStore.h"
#include "utils/StmtEntity.h"
#include <unordered_set>
#include <unordered_map>
#include "catch.hpp"

TEST_CASE("Test storing few statements") {
	StatementStore statementStore = StatementStore();
	statementStore.addStatements({ { 1, StmtEntity::kAssign }, { 2, StmtEntity::kAssign }, { 3, StmtEntity::kAssign } });

	REQUIRE(statementStore.getStatements(StmtEntity::kAssign) == std::unordered_set<int>{1, 2, 3});
}

TEST_CASE("Test storing few statements with different types") {
	StatementStore statementStore = StatementStore();
	statementStore.addStatements({ { 1, StmtEntity::kAssign }, { 2, StmtEntity::kWhile }, { 3, StmtEntity::kAssign } });

	REQUIRE(statementStore.getStatements(StmtEntity::kAssign) == std::unordered_set<int>{1, 3});
	REQUIRE(statementStore.getStatements(StmtEntity::kWhile) == std::unordered_set<int>{2});
}