#include "catch.hpp"
#include <memory>
#include <unordered_set>
#include <string>

#include "utils/hash_utils.h"
#include "SP/TNode.h"
#include "SP/SourceProcessor.h"
#include "PKB/API/WriteFacade.h"
#include "PKB/API/ReadFacade.h"
#include "qps/qps.h"

TEST_CASE("TEST Assign Pattern Exact Match") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  QPS qps(readFacade);

  std::string simpleProgram = "procedure foo { y = z; }";

  sourceProcessor.processSource(simpleProgram);

  ExactExpr rhs = ExactExpr{"z"};

  REQUIRE(readFacade.getAssignPair(rhs) ==
      std::unordered_set<std::pair<statementNumber, variable>, PairHash>({{1, "y"}}));
}

TEST_CASE("TEST Assign Pattern Partial Match") {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  QPS qps(readFacade);

  std::string simpleProgram = "procedure foo { x = v + x * y + z * t; }";
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> result = {{1, "x"}};
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> empty = {};
  sourceProcessor.processSource(simpleProgram);

  REQUIRE(readFacade.getAssignPair(PartialExpr{"v"}) == result);
  REQUIRE(readFacade.getAssignPair(PartialExpr{"x*y"}) == result);
  REQUIRE(readFacade.getAssignPair(PartialExpr{"v+x"}) == empty);
  REQUIRE(readFacade.getAssignPair(PartialExpr{"v+x*y"}) == result);
  REQUIRE(readFacade.getAssignPair(PartialExpr{"y+z*t"}) == empty);
  REQUIRE(readFacade.getAssignPair(PartialExpr{"x*y+z*t"}) == empty);
  REQUIRE(readFacade.getAssignPair(PartialExpr{"v+x*y+z*t"}) == result);
}
