#include "SP/SimpleParser.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("Test SimpleParser") {

    SimpleParser parser;

    REQUIRE(parser.parse() == 0);
}
