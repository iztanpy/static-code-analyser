#include "SP/TNode.h"
#include "catch.hpp"
#include "SP/SimpleParser.h"
#include "SP/DesignExtractor.h"
#include "SP/SPTokeniser.h"
#include "PKB/API/WriteFacade.h"
#include "PKB/API/ReadFacade.h"


#include "catch.hpp"

using namespace std;

TEST_CASE("1") {
    SPtokeniser tokeniser;
    PKB pkb = PKB();
    ReadFacade readFacade = ReadFacade(&pkb);
    WriteFacade writeFacade = WriteFacade(&pkb);
    SimpleParser parser(&writeFacade);

    // Initialize SP and SP tokenizer
    string simpleProgram = "x = x + 1;";
    parser.tokenise(simpleProgram);
    
    unordered_set<string> varSet = unordered_set<string>({"x"});
    unordered_map<string, unordered_set<string>> varUseMap = unordered_map<string, unordered_set<string>>(
            {{"x", varSet}});
    unordered_set<string> constSet = unordered_set<string>({"1"});
    unordered_map<string, unordered_set<string>> constUseMap = unordered_map<string, unordered_set<string>>(
            {{"x", constSet}});

    REQUIRE(parser.assignmentParser->getAssignVarHashmap() == varUseMap);
    REQUIRE(parser.assignmentParser->getAssignConstHashmap() == constUseMap);
    REQUIRE(parser.assignmentParser->getVariablesHashset() == varSet);
    REQUIRE(parser.assignmentParser->getConstantsHashset() == constSet);

  
    REQUIRE(readFacade.getAllVariables() == std::unordered_set<std::string>{"x"});
    REQUIRE(readFacade.getAllConstants() == std::unordered_set<std::string>{"1"});
}

TEST_CASE("2") {
    SPtokeniser tokeniser;
    PKB pkb = PKB();
    ReadFacade readFacade = ReadFacade(&pkb);
    WriteFacade writeFacade = WriteFacade(&pkb);
    SimpleParser parser(&writeFacade);

    string simpleProgram = "x = y + 1;";
    parser.tokenise(simpleProgram);
    REQUIRE(readFacade.getAllVariables() == std::unordered_set<std::string>{"x", "y"});
    REQUIRE(readFacade.getAllConstants() == std::unordered_set<std::string>{"1"});
}

TEST_CASE("3") {
    SPtokeniser tokeniser;
    PKB pkb = PKB();
    ReadFacade readFacade = ReadFacade(&pkb);
    WriteFacade writeFacade = WriteFacade(&pkb);
    SimpleParser parser(&writeFacade);

    string simpleProgram = "x = 3; y = 4;";
    parser.tokenise(simpleProgram);
    REQUIRE(readFacade.getAllVariables() == std::unordered_set<std::string>{"x", "y"});
    REQUIRE(readFacade.getAllConstants() == std::unordered_set<std::string>{"3", "4"});
}


TEST_CASE("4") {
    SPtokeniser tokeniser;
    PKB pkb = PKB();
    ReadFacade readFacade = ReadFacade(&pkb);
    WriteFacade writeFacade = WriteFacade(&pkb);
    SimpleParser parser(&writeFacade);

    string simpleProgram = "x = z - 3; y = y + 4;";
    parser.tokenise(simpleProgram);
    REQUIRE(readFacade.getAllVariables() == std::unordered_set<std::string>{"x", "y", "z"});
    REQUIRE(readFacade.getAllConstants() == std::unordered_set<std::string>{"3", "4"});
}



