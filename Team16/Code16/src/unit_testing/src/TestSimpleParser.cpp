#include "SP/SimpleParser.h"
#include "SP/DesignExtractor.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("Test SimpleParser") {

    SimpleParser parser;
    // line 0: x = x + 1
    Token tokenX = Token("variable", 0, "x");
    Token tokenEqual = Token("equal", 0);
    Token tokenX2 = Token("variable", 0, "x");
    Token tokenPlus = Token("plus", 0);
    Token token1 = Token("constant", 0, 1);
    Token tokenEnd = Token(";");
    std::vector<Token> my_tokens{tokenX, tokenEqual, tokenX2, tokenPlus, token1, tokenEnd};


    REQUIRE(parser.parse(my_tokens) == 6);
}

TEST_CASE("Test DesignExtractor1") { // x = x + 1

    // line 0: x = x + 1
    TNode nodeX = TNode("variable", "x");
    TNode nodeEqual = TNode("equal");
    TNode nodeX2 = TNode("variable", "x");
    TNode nodePlus = TNode("plus");
    TNode node1 = TNode("constant", "1");

    nodeEqual.addChild(nodeX);
    nodePlus.addChild(nodeX2);
    nodePlus.addChild(node1);
    nodeEqual.addChild(nodePlus);

    DesignExtractor de = *new DesignExtractor();
    de.extractDesign(nodeEqual);

    unordered_set<string> varSet = unordered_set<string>({"x"});
    unordered_map<string, unordered_set<string>> varUseMap = unordered_map<string, unordered_set<string>>({{"x", varSet}});

    REQUIRE(de.populatePKB() == varUseMap);
}


TEST_CASE("Test DesignExtractor3") { // x = x + y + w

    TNode nodeX = TNode("variable", "x");
    TNode nodeEqual = TNode("equal");
    TNode nodeX2 = TNode("variable", "x");
    TNode nodePlus = TNode("plus");
    TNode nodey = TNode("variable", "y");
    TNode nodePlus2 = TNode("plus");
    TNode nodew = TNode("variable", "w");


    nodeEqual.addChild(nodeX);
    nodePlus.addChild(nodeX2);
    nodePlus2.addChild(nodey);
    nodePlus2.addChild(nodew);
    nodePlus.addChild(nodePlus2);
    nodeEqual.addChild(nodePlus);

    DesignExtractor de = *new DesignExtractor();
    de.extractDesign(nodeEqual);

    unordered_set<string> varSet = unordered_set<string>({"x", "y", "w"});
    unordered_map<string, unordered_set<string>> varUseMap = unordered_map<string, unordered_set<string>>({{"x", varSet}});

    REQUIRE(de.populatePKB() == varUseMap);
}


TEST_CASE("Test DesignExtractor4") { // x = x + 1 + w

    // line 0: x = x + 1
    TNode nodeX = TNode("variable", "x");
    TNode nodeEqual = TNode("equal");
    TNode nodeX2 = TNode("variable", "x");
    TNode nodePlus = TNode("plus");
    TNode node1 = TNode("constant", "1");
    TNode nodePlus2 = TNode("plus");
    TNode nodew = TNode("variable", "w");


    nodeEqual.addChild(nodeX);
    nodePlus.addChild(nodeX2);
    nodePlus2.addChild(node1);
    nodePlus2.addChild(nodew);
    nodePlus.addChild(nodePlus2);
    nodeEqual.addChild(nodePlus);

    DesignExtractor de = *new DesignExtractor();
    de.extractDesign(nodeEqual);

    unordered_set<string> varSet = unordered_set<string>({"x", "w"});
    unordered_map<string, unordered_set<string>> varUseMap = unordered_map<string, unordered_set<string>>({{"x", varSet}});

    REQUIRE(de.populatePKB() == varUseMap);
}