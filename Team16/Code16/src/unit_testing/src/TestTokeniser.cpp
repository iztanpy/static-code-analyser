
#include "SP/SPTokeniser.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("Test Tokeniser") {
    SPtokeniser tokeniser; // Use the correct class name
    std::vector<std::string> tokens = tokeniser.tokenise("Hello, world!");
    std::vector<std::string> expected_tokens = { "Hello,",  "world!"};
    REQUIRE(tokens == expected_tokens); 
}
