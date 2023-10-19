//
// Created by Isaac Tan on 19/10/23.
//
#include <unordered_set>
#include "catch.hpp"
#include "PKB/Stores/NextStore.h"
#include "utils/entity_types.h"
#include "SP/SourceProcessor.h"
#include "PKB/API/WriteFacade.h"

TEST_CASE("Test Next store") {
    auto nextStore = NextStore();
    nextStore.storeNext({{1, {2, 3}}, {2, {4, 5}}, {3, {6, 7}}, {6, {8, 9}}});
    REQUIRE(nextStore.isNext(1,2));
    REQUIRE(nextStore.isNext(Wildcard(), 2));
    REQUIRE(nextStore.isNext(1, Wildcard()));
    REQUIRE(nextStore.isNext(Wildcard(), Wildcard()));
    REQUIRE(!nextStore.isNext(1, 4));
    REQUIRE(!nextStore.isNext(9, Wildcard()));
    REQUIRE(!nextStore.isNext(Wildcard(), 1));
}

TEST_CASE("Test NextStar Store") {
    auto nextStore = NextStore();
    auto pkb = PKB();
    auto facade = WriteFacade(pkb);
    auto sourceProcessor = SourceProcessor(&facade);
    std::string simpleProgram4 = R"(procedure Second {
        while (x==0) {
            if (x==1) then {
                help = help +1;
            } else {
                if (x == 2) then {
                     help = help +1;
                     help = help +2;
                     help = help +3;
                }
                test = test + 1;
            }
        }
        a = a + b;
      })";
    sourceProcessor.processSource(simpleProgram4);

}



