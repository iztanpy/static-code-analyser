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

//TEST_CASE("Test Local") {
//  std::string filename = "../../../tests/Demo_source.txt";
//  std::string query = "variable v; Select v";
//
//  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
//  SourceProcessor sourceProcessor(&writeFacade);
//
//  parse(sourceProcessor, filename);
//
//  ReadFacade readFacade = ReadFacade(*pkb_ptr);
//  QPS qps(readFacade);
//
//  std::unordered_set<std::string> results = qps.Evaluate(query);
//
//  REQUIRE(results == std::unordered_set<std::string>({"i", "x", "y", "z"}));
//}
                          "        while (3 == 4) {\n"
