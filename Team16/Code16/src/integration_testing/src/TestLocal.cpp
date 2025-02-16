#include "catch.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include "PKB/API/read_facade.h"
#include "PKB/API/write_facade.h"
#include "PKB/pkb.h"
#include "qps/qps.h"
#include "SP/source_processor.h"

void parse(SourceProcessor& sp, std::string filename) {
  std::ifstream file(filename);
  std::string file_contents;

  if (!file.is_open()) {
    std::cerr << "error: Unable to open file " << filename << std::endl;
  }

  std::string input;
  std::string line;
  while (std::getline(file, line)) {
    input += line;
  }
  sp.processSource(input);
}

//
//TEST_CASE("Test Local") {
//  std::string filename = "../../../tests/Sample_source.txt";
//  std::string query = "procedure p, p1,p2; variable v,v1; if if; read r; while while;\n"
//                      "Select <p2.procName,v1> such that Modifies(p2,v1) such that Calls(p,p1) and Calls*(p1,p2) pattern if(v,_,_) and while(v,_) with r.varName = v.varName";
//
//  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//  write_facade writeFacade = write_facade(*pkb_ptr);
//  SourceProcessor sourceProcessor(&writeFacade);
//
//  parse(sourceProcessor, filename);
//
//  ReadFacade readFacade = ReadFacade(*pkb_ptr);
//  QPS qps(readFacade);
//
//  std::unordered_set<std::string> results = qps.Evaluate(query);
//
//  REQUIRE(results == std::unordered_set<std::string>({"SemanticError"}));
//}
