#include <memory>

#include "SP/TNode.h"
#include "catch.hpp"
#include "SP/SimpleParser.h"
#include "PKB/API/WriteFacade.h"
#include "PKB/API/ReadFacade.h"
#include "qps/qps.h"

using namespace std;

TEST_CASE("One assign statement 1") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SimpleParser parser(&writeFacade);
  QPS qps(readFacade);

  // Initialize SP and SP tokenizer
  string simpleProgram = "x = x + 1;";
  string query_1 = "variable v; Select v";
  string query_2 = "constant c; Select c";

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

  REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({"x"}));
  REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({"1"}));
}

TEST_CASE("One assign statement with white-spaces") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SimpleParser parser(&writeFacade);
  QPS qps(readFacade);


  string simpleProgram = "procedure test { \n x = \t \n \n\t y + \t 1; \n }";
  string query_1 = "variable v\t\t\n; Select \n v";
  string query_2 = "\n\t constant \t c; Select c";

  parser.tokenise(simpleProgram);

  REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({"x", "y"}));
  REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({"1"}));
}


TEST_CASE("Simple assign statements") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SimpleParser parser(&writeFacade);
  QPS qps(readFacade);

  string simpleProgram = "x = 3; y = 4;";
  string query_1 = "variable v; Select v";
  string query_2 = "constant c; Select c";

  parser.tokenise(simpleProgram);

  REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({"x", "y"}));
  REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({"3", "4"}));
}

TEST_CASE("Multiple assign statements") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SimpleParser parser(&writeFacade);
  QPS qps(readFacade);

  string simpleProgram = "x = z - 3; y = y + 4; i = i + 10;";
  string query_1 = "variable v; Select v";
  string query_2 = "constant c; Select c";

  parser.tokenise(simpleProgram);

  REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({"x", "y", "z", "i"}));
  REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({"3", "4", "10"}));
}

TEST_CASE("Assign statements with many SIMPLE RHS terms and whitespaces") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SimpleParser parser(&writeFacade);
  QPS qps(readFacade);

  string simpleProgram = "x = z - 3 + I - \n 100 + \t u100 + U48ka - \n \t OoOhd; \t  y = y + 4;";
  string query_1 = "variable v; Select v";
  string query_2 = "constant c; Select c";

  parser.tokenise(simpleProgram);

  REQUIRE(qps.Evaluate(query_1)
              == std::unordered_set<std::string>({"x", "y", "z", "I", "u100", "U48ka", "OoOhd"}));
  REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({"3", "4", "100"}));
}

TEST_CASE("Assign statements with mixed-case PQL synonyms & many declarations") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SimpleParser parser(&writeFacade);
  QPS qps(readFacade);

  string simpleProgram = "procedure c {x = z - 3 + I - \n 100 + \t u100 + U48ka - \n \t OoOhd;} procedure procedure "
                         "{ \t  y = y + 4;}";
  string query_1 = "variable Vj5u, v39, constant OvO; Select Vj5u";
  string query_2 = "constant c10ueYwh, variable Ur; Select c10ueYwh";

  parser.tokenise(simpleProgram);
  REQUIRE(qps.Evaluate(query_1)
              == std::unordered_set<std::string>({"x", "y", "z", "I", "u100", "U48ka", "OoOhd"}));
  REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({"3", "4", "100"}));
}

TEST_CASE("Selecting Assign statements") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SimpleParser parser(&writeFacade);
  QPS qps(readFacade);

  string simpleProgram = "procedure c {x = z - 3 + I - \n 100 + \t u100 + U48ka - \n \t OoOhd;} procedure procedure "
                         "{ \t  read r; y = y + 4;}";
  string query_1 = "assign a, Select a";

  parser.tokenise(simpleProgram);
  REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({"1", "3"}));
}

//TEST_CASE("Uses test") {
//  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//
//  ReadFacade readFacade = ReadFacade(*pkb_ptr);
//  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
//  SimpleParser parser(&writeFacade);
//  QPS qps(readFacade);
//
//  string simpleProgram = "procedure c {x = z - 3 + I; x = x + 1; y = y + 4;}";
//  string query_1 = "variable v; assign a; Select v such that Uses(1, v)";
//
//  parser.tokenise(simpleProgram);
//  REQUIRE(readFacade.getAllVariables() == std::unordered_set<std::string>{"x", "z", "I", "y"});
//  REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({"1", "3"}));
//}
