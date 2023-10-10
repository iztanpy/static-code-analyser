#include "catch.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include "PKB/API/ReadFacade.h"
#include "PKB/API/WriteFacade.h"
#include "PKB/PKB.h"
#include "qps/qps.h"
#include "SP/SourceProcessor.h"

void parse(SourceProcessor& sp, std::string filename) {
  std::ifstream file(filename);
  std::string file_contents;

  if (!file.is_open()) {
    std::cerr << "Error: Unable to open file " << filename << std::endl;
  }

  std::string input;
  std::string line;
  while (std::getline(file, line)) {
    /*    std::cout << "TEST" << std::endl;*/
    input += line;
  }
  sp.processSource(input);
}

TEST_CASE("Test Local") {
  std::string filename = "/Users/isaactan/Documents/GitHub/23s1-cp-spa-team-16/Team16/Tests16/Milestone2/Basic_Spa/Assign_Patterns_All_Combinations_source.txt";
  std::string query = "assign a; variable v; Select a pattern a(_, _\"k - 1 + i * 3 / 2\"_)";

  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);

  parse(sourceProcessor, filename);

  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  QPS qps(readFacade);

  std::unordered_set<std::string> results = qps.Evaluate(query);

  REQUIRE(results == std::unordered_set<std::string>({"2", "7"}));
}