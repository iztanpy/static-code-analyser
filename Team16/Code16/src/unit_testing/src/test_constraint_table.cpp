#include "catch.hpp"

#include "qps/query_evaluator/constraint_table.h"

using namespace std;
void require_l(bool b) {
  REQUIRE(b);
}

TEST_CASE("ConstraintTable::AddNewUnaryConstraint", "[ConstraintTable]") {
  // TODO(phuccuongngo99): Add test case where there's nothing inside yet
  ConstraintTable ct(
      {
          {"a", {"1", "2"}},
          {"b", {"3", "4"}},
      }
  );

  SECTION("Adding a new unary constraint") {
    ct.AddNewUnaryConstraint("c", {"4", "5"});
    auto table = ct.table;
    REQUIRE(table["a"] == Col{"1", "1", "2", "2"});
    REQUIRE(table["b"] == Col{"3", "3", "4", "4"});
    REQUIRE(table["c"] == Col{"4", "5", "4", "5"});
  }
}

TEST_CASE("ConstraintTable::AddNewBinaryConstraint", "[ConstraintTable]") {
  // TODO(phuccuongngo99): Add test case where there's nothing inside yet
  ConstraintTable ct(
      {
          {"a", {"1", "2"}},
          {"b", {"3", "4"}},
      }
  );

  SECTION("Adding a new unary constraint") {
    ct.AddNewBinaryConstraint("c", {"6", "8", "10"}, "d", {"7", "9", "11"});
    auto table = ct.table;
    REQUIRE(table["a"] == Col{"1", "1", "1", "2", "2", "2"});
    REQUIRE(table["b"] == Col{"3", "3", "3", "4", "4", "4"});
    REQUIRE(table["c"] == Col{"6", "8", "10", "6", "8", "10"});
    REQUIRE(table["d"] == Col{"7", "9", "11", "7", "9", "11"});
  }
}

TEST_CASE("ConstraintTable::AddExistingUnaryConstraint", "[ConstraintTable]") {
  // TODO(phuccuongngo99): Add test case where there's nothing inside yet
  ConstraintTable ct(
      {
          {"a", {"1", "2", "8", "9"}},
          {"b", {"3", "4", "10", "11"}},
          {"c", {"5", "6", "12", "13"}},
      }
  );

  SECTION("Adding a new unary constraint") {
    ct.AddExistingUnaryConstraint("a", {"1", "2", "9"});
    REQUIRE(ct.table["a"] == Col{"1", "2", "9"});
    REQUIRE(ct.table["b"] == Col{"3", "4", "11"});
    REQUIRE(ct.table["c"] == Col{"5", "6", "13"});

    ct.AddExistingUnaryConstraint("c", {"5", "13", "14", "1"});
    REQUIRE(ct.table["a"] == Col{"1", "9"});
    REQUIRE(ct.table["b"] == Col{"3", "11"});
    REQUIRE(ct.table["c"] == Col{"5", "13"});
  }
}

TEST_CASE("ConstraintTable::AddExistingBinaryConstraint", "[ConstraintTable]") {
  // TODO(phuccuongngo99): Add test case where there's nothing inside yet
  ConstraintTable ct(
      {
          {"a", {"1", "2", "8", "9"}},
          {"b", {"3", "4", "10", "11"}},
          {"c", {"5", "6", "12", "13"}},
      }
  );

  SECTION("Adding a new unary constraint") {
    ct.AddExistingBinaryConstraint("a", {"1", "2", "100", "9"}, "b", {"3", "4", "11", "8"});
    REQUIRE(ct.table["a"] == Col{"1", "2"});
    REQUIRE(ct.table["b"] == Col{"3", "4"});
    REQUIRE(ct.table["c"] == Col{"5", "6"});
  }
}

TEST_CASE("ConstraintTable::AddHalfExistingBinaryConstraint", "[ConstraintTable]") {
  // TODO(phuccuongngo99): Add test case where there's nothing inside yet
  ConstraintTable ct(
      {
          {"a", {"1", "2", "8", "9"}},
          {"b", {"3", "4", "10", "11"}},
          {"c", {"5", "6", "12", "13"}},
      }
  );

  SECTION("Adding a new unary constraint") {
    ct.AddHalfExistingBinaryConstraint("a", {"1", "1", "3"}, "d", {"2", "4", "11"});
    REQUIRE(ct.table["a"] == Col{"1", "1"});
    REQUIRE(ct.table["b"] == Col{"3", "3"});
    REQUIRE(ct.table["c"] == Col{"5", "5"});
    REQUIRE(ct.table["d"] == Col{"2", "4"});
  }
}
