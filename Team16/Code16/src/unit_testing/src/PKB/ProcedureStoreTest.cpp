#include "PKB/Stores/procedure_store.h"
#include <unordered_set>
#include <set>
#include "catch.hpp"

TEST_CASE("Test storing procedures") {
  procedure_store procedureStore = procedure_store();
  procedureStore.addProcedures({"proc1", "proc2", "proc3"});

  for (auto procedure : procedureStore.getAllProcedures()) {
    REQUIRE((procedure == "proc1" || procedure == "proc2" || procedure == "proc3"));
  }

  procedureStore.addProcedures({{"proc1", {1, 20}}, {"proc2", {21, 30}}, {"proc3", {31, 40}}});

  REQUIRE(procedureStore.getProcedureRange("proc1") == std::make_pair(1, 20));
  REQUIRE(procedureStore.getProcedureRange("proc2") == std::make_pair(21, 30));
  REQUIRE(procedureStore.getProcedureRange("proc3") == std::make_pair(31, 40));

}