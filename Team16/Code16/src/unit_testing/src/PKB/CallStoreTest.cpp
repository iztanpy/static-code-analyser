#include <stdio.h>
#include <unordered_set>
#include "catch.hpp"
#include "PKB/Stores/call_store.h"

typedef std::string procedure;

TEST_CASE("Test call_store Functionality") {
  call_store callStore = call_store();

  callStore.storeCalls({{"main", {"proc1", "proc2"}}, {"proc1", {"proc2"}}, {"proc2", {"proc3"}}});

  REQUIRE(callStore.getCallChildren("main") == std::unordered_set<procedure>({"proc1", "proc2"}));
  REQUIRE(callStore.getCallChildren("proc1") == std::unordered_set<procedure>({"proc2"}));
  REQUIRE(callStore.getCallChildren("proc2") == std::unordered_set<procedure>({"proc3"}));
  REQUIRE(callStore.getCallChildren("proc3") == std::unordered_set<procedure>({}));

  REQUIRE(callStore.getCallStarChildren("main") == std::unordered_set<procedure>({"proc1", "proc2", "proc3"}));
  REQUIRE(callStore.getCallStarChildren("proc1") == std::unordered_set<procedure>({"proc2", "proc3"}));
  REQUIRE(callStore.getCallStarChildren("proc2") == std::unordered_set<procedure>({"proc3"}));

  REQUIRE(callStore.getCallStarParents("main") == std::unordered_set<procedure>({}));
  REQUIRE(callStore.getCallStarParents("proc1") == std::unordered_set<procedure>({"main"}));
  REQUIRE(callStore.getCallStarParents("proc2") == std::unordered_set<procedure>({"main", "proc1"}));
  REQUIRE(callStore.getCallStarParents("proc3") == std::unordered_set<procedure>({"main", "proc1", "proc2"}));
}

TEST_CASE("Test call_store Functionality 2") {
  call_store callStore = call_store();
  REQUIRE_THROWS(callStore.storeCalls({{"main", {"proc1", "proc2"}}, {"proc1", {"proc2"}}, {"proc2", {"main"}}}));
}