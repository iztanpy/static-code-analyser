#include <unordered_set>
#include <unordered_map>
#include "catch.hpp"
#include "PKB/Stores/UsesStore.h"
#include "utils/entity_types.h"

typedef int statementNumber;
typedef std::string variable;

TEST_CASE("Test Uses Store Add") {
  auto usesStore = UsesStore();
  usesStore.storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>>
                          {{1, {"x"}}, {2, {"x", "y"}}, {3, {"y"}}});
  REQUIRE(usesStore.uses(1) == std::unordered_set<variable>{"x"});
  REQUIRE(usesStore.uses(2) == std::unordered_set<variable>{"x", "y"});
  REQUIRE(usesStore.uses(3) == std::unordered_set<variable>{"y"});
}

TEST_CASE("Test Uses store Uses") {
  auto usesStore = UsesStore();
  usesStore.storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>>
                          {{1, {"x"}}, {2, {"x", "y"}}, {3, {"y"}}});
  std::unordered_set<statementNumber> statements = usesStore.uses("x");
  REQUIRE(statements == std::unordered_set<statementNumber>{1, 2});
  statements = usesStore.uses("y");
  REQUIRE(statements == std::unordered_set<statementNumber>{2, 3});

  REQUIRE(usesStore.isUses(1, "x"));
  REQUIRE(usesStore.isUses(2, "x"));
  REQUIRE(usesStore.isUses(2, "y"));
  REQUIRE(usesStore.isUses(3, "y"));
  REQUIRE(!usesStore.isUses(1, "y"));
  REQUIRE(!usesStore.isUses(3, "x"));

  Wildcard wildcard = Wildcard();

  REQUIRE(usesStore.isUses(1));
  REQUIRE(usesStore.isUses(2));
  REQUIRE(usesStore.isUses(3));
  REQUIRE(!usesStore.isUses(4));

  REQUIRE(usesStore.uses(1) == std::unordered_set<variable>{"x"});
  REQUIRE(usesStore.uses(2) == std::unordered_set<variable>{"x", "y"});
  REQUIRE(usesStore.uses(3) == std::unordered_set<variable>{"y"});

  REQUIRE(usesStore.uses("x") == std::unordered_set<statementNumber>{1, 2});
  REQUIRE(usesStore.uses("y") == std::unordered_set<statementNumber>{2, 3});

}