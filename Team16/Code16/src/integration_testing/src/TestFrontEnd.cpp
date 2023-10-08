#include <memory>
#include <unordered_set>
#include <string>
#include "SP/TNode.h"
#include "catch.hpp"
#include "SP/SourceProcessor.h"
#include "PKB/API/WriteFacade.h"
#include "PKB/API/ReadFacade.h"
#include "qps/qps.h"

TEST_CASE("One print statement 1") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  QPS qps(readFacade);

  // Initialize SP and SP tokenizer
  std::string simpleProgram = "procedure k {print k;}";
  std::string query_1 = "variable v; Select v";
  sourceProcessor.processSource(simpleProgram);

  std::unordered_set<std::string> varSet = std::unordered_set<std::string>({"k"});
  std::unordered_map<int, std::unordered_set<std::string>> usesLineVariable =
      std::unordered_map<int, std::unordered_set<std::string>>({{1, {"k"}}});
  REQUIRE(sourceProcessor.getVariables() == varSet);
  REQUIRE(sourceProcessor.getUsesLineRHSVarMap() == usesLineVariable);
  REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({"k"}));

}

TEST_CASE("One read statement 1") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  QPS qps(readFacade);

  // Initialize SP and SP tokenizer
  std::string simpleProgram = "procedure poo{read k;}";
  std::string query_1 = "variable v; Select v";
  sourceProcessor.processSource(simpleProgram);
  REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({"k"}));

  std::unordered_set<std::string> varSet = std::unordered_set<std::string>({"k"});
  std::unordered_map<int, std::unordered_set<std::string>>
      modifiesLineVariable = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"k"}}});
  REQUIRE(sourceProcessor.getVariables() == varSet);
}

TEST_CASE("Test SP-PKB connection") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);

  std::string simpleProgram = "procedure p {x = z - 3 + I; x = x + 1; y = y + z + 4;}";

  sourceProcessor.processSource(simpleProgram);

  REQUIRE((readFacade.getAllAssigns() == std::unordered_set<statementNumber>({1, 2, 3})));

  REQUIRE((readFacade.getAssignPair(PartialExpr{"x"})
      == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({{2, "x"}})));

  REQUIRE((readFacade.getAssignPair(PartialExpr{"y"})
      == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({{3, "y"}})));

  REQUIRE((readFacade.getAssignPair(PartialExpr{"z"})
      == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({{1, "x"}, {3, "y"}})));

  Wildcard wildcard = Wildcard();

  REQUIRE((readFacade.getAssignPair(wildcard)
      == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({{1, "x"}, {2, "x"}, {3, "y"}})));

  REQUIRE((readFacade.getAssigns(wildcard, PartialExpr{"x"}) == std::unordered_set<statementNumber>({2})));

  REQUIRE((readFacade.getAssigns(wildcard, PartialExpr{"y"}) == std::unordered_set<statementNumber>({3})));

  REQUIRE((readFacade.getAssigns(wildcard, PartialExpr{"z"}) == std::unordered_set<statementNumber>({1, 3})));

  REQUIRE((readFacade.getAssigns(wildcard, wildcard) == std::unordered_set<statementNumber>({1, 2, 3})));

  REQUIRE((readFacade.getAssigns("x", PartialExpr{"z"}) == std::unordered_set<statementNumber>({1})));

  REQUIRE((readFacade.getAssigns("x", wildcard) == std::unordered_set<statementNumber>({1, 2})));

  REQUIRE((readFacade.isUses(1, "z")));

  REQUIRE((readFacade.isUses(1, "I")));

  REQUIRE(!(readFacade.isUses(1, "x")));

  REQUIRE((readFacade.isUses(2, "x")));

  REQUIRE((readFacade.isUses(3, "y")));

  REQUIRE((readFacade.isUses(3, "z")));

  REQUIRE((readFacade.uses(StmtEntity::kAssign, "z") == std::unordered_set<statementNumber>({1, 3})));

  REQUIRE((readFacade.uses(StmtEntity::kAssign, "x") == std::unordered_set<statementNumber>({2})));

  REQUIRE((readFacade.uses(StmtEntity::kAssign, "y") == std::unordered_set<statementNumber>({3})));

  REQUIRE((readFacade.uses(StmtEntity::kAssign, wildcard) == std::unordered_set<statementNumber>({1, 2, 3})));

  std::unordered_set<std::pair<statementNumber, variable>, PairHash> pairSet =
      std::unordered_set<std::pair<statementNumber, variable>, PairHash>({{1, "z"}, {1, "I"}, {2, "x"}, {3, "y"},
                                                                          {3, "z"}});

  REQUIRE((readFacade.uses(StmtEntity::kAssign) == pairSet));

  REQUIRE(readFacade.getVariables() == std::unordered_set<variable>({"x", "z", "I", "y"}));

  REQUIRE(readFacade.modifies(1) == std::unordered_set<variable>({"x"}));
  REQUIRE(readFacade.modifies(2) == std::unordered_set<variable>({"x"}));
  REQUIRE(readFacade.modifies(3) == std::unordered_set<variable>({"y"}));

  REQUIRE(readFacade.uses(1) == std::unordered_set<variable>({"z", "I"}));
  REQUIRE(readFacade.uses(2) == std::unordered_set<variable>({"x"}));
  REQUIRE(readFacade.uses(3) == std::unordered_set<variable>({"y", "z"}));
}

TEST_CASE("One assign statement 1") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  QPS qps(readFacade);

  // Initialize SP and SP tokenizer
  std::string simpleProgram = "procedure p {x = x + 1;}";
  std::string query_1 = "variable v; Select v";
  std::string query_2 = "constant c; Select c";

  sourceProcessor.processSource(simpleProgram);

  std::unordered_set<std::string> varSet = std::unordered_set<std::string>({"x"});
  std::unordered_map<std::string, std::unordered_set<std::string>>
      varUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>(
      {{"x", varSet}});
  std::unordered_set<std::string> constSet = std::unordered_set<std::string>({"1"});
  std::unordered_map<std::string, std::unordered_set<std::string>>
      constUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>(
      {{"x", constSet}});
  std::unordered_map<int, std::unordered_set<std::string>>
      usesLineRHSPatternMap = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"x", "1", "x + 1"}}});
  std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>({{1, "x"}});
  std::unordered_map<int, std::unordered_set<std::string>>
      usesLineRHSVarMap = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"x"}}});

  REQUIRE(sourceProcessor.getUsesLineRHSPatternMap() == usesLineRHSPatternMap);
  REQUIRE(sourceProcessor.getUsesLineLHSMap() == usesLineLHSMap);
  REQUIRE(sourceProcessor.getUsesLineRHSVarMap() == usesLineRHSVarMap);
  REQUIRE(sourceProcessor.getVariables() == varSet);
  REQUIRE(sourceProcessor.getConstants() == constSet);

  REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({"x"}));
  REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({"1"}));
}

TEST_CASE("One assign statement with white-spaces") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  QPS qps(readFacade);

  std::string simpleProgram = "procedure test { \n x = \t \n \n\t y + \t 1; \n }";
  std::string query_1 = "variable v\t\t\n; Select \n v";
  std::string query_2 = "\n\t constant \t c; Select c";

  sourceProcessor.processSource(simpleProgram);

  REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({"x", "y"}));
  REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({"1"}));
}

TEST_CASE("Simple assign statements") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  QPS qps(readFacade);

  std::string simpleProgram = "procedure p {x = 3; y = 4;}";
  std::string query_1 = "variable v; Select v";
  std::string query_2 = "constant c; Select c";

  sourceProcessor.processSource(simpleProgram);

  REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({"x", "y"}));
  REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({"3", "4"}));
}

TEST_CASE("Multiple assign statements") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  QPS qps(readFacade);

  std::string simpleProgram = "procedure p{x = z - 3; y = y + 4; i = i + 10;}";
  std::string query_1 = "variable v; Select v";
  std::string query_2 = "constant c; Select c";

  sourceProcessor.processSource(simpleProgram);

  REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({"x", "y", "z", "i"}));
  REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({"3", "4", "10"}));
}

TEST_CASE("Assign statements with many SIMPLE RHS terms and whitespaces") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  QPS qps(readFacade);
  std::string simpleProgram = "procedure p{x = z - 3 + I - \n 100 + \t u100 + U48ka - \n \t OoOhd; \t  y = y + 4; }";
  std::string query_1 = "variable v; Select v";
  std::string query_2 = "constant c; Select c";

  sourceProcessor.processSource(simpleProgram);

  REQUIRE(qps.Evaluate(query_1)
              == std::unordered_set<std::string>({"x", "y", "z", "I", "u100", "U48ka", "OoOhd"}));
  REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({"3", "4", "100"}));
}

TEST_CASE("Assign statements with mixed-case PQL synonyms & many declarations") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  QPS qps(readFacade);

  std::string
      simpleProgram = "procedure c {x = z - 3 + I - \n 100 + \t u100 + U48ka - \n \t OoOhd;} procedure procedure "
                      "{ \t  y = y + 4;}";
  std::string query_1 = "variable Vj5u, v39; constant OvO; Select Vj5u";
  std::string query_2 = "constant c10ueYwh; variable Ur; Select c10ueYwh";

  sourceProcessor.processSource(simpleProgram);
  REQUIRE(qps.Evaluate(query_1)
              == std::unordered_set<std::string>({"x", "y", "z", "I", "u100", "U48ka", "OoOhd"}));
  REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({"3", "4", "100"}));
}

TEST_CASE("Selecting Assign statements") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  QPS qps(readFacade);

  std::string
      simpleProgram = "procedure c {x = z - 3 + I - \n 100 + \t u100 + U48ka - \n \t OoOhd;} procedure procedure "
                      "{ \t  read r; y = y + 4;}";
  std::string query_1 = "assign a; Select a";

  sourceProcessor.processSource(simpleProgram);
  REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({"1", "3"}));
}