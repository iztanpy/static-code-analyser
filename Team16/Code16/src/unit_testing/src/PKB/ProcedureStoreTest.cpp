#include "PKB/Stores/ProcedureStore.h"
#include <unordered_set>
#include <set>
#include "catch.hpp"

TEST_CASE("Test storing procedures") {
	ProcedureStore procedureStore = ProcedureStore();
	procedureStore.addProcedures({"proc1", "proc2", "proc3"});

	for (auto procedure : procedureStore.getAllProcedures()) {
		REQUIRE((procedure == "proc1" || procedure == "proc2" || procedure == "proc3"));
	}
}