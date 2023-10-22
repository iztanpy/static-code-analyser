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
  std::string filename = "procedure A {\n"
                         "    read a;\n"
                         "    print b;\n"
                         "    while (c > d + 3) {\n"
                         "        if (e == f - 2) then {\n"
                         "            call B;\n"
                         "        } else {\n"
                         "            g = h + 4;\n"
                         "        }\n"
                         "    }\n"
                         "    call C;\n"
                         "    i = j * 2;\n"
                         "}\n"
                         "\n"
                         "procedure B {\n"
                         "    read k;\n"
                         "    print l;\n"
                         "    m = n / 2;\n"
                         "    if (o > p) then {\n"
                         "        print q;\n"
                         "    } else {\n"
                         "        call D;\n"
                         "    }\n"
                         "    r = s - 1;\n"
                         "}\n"
                         "\n"
                         "procedure C {\n"
                         "    print t;\n"
                         "    u = v + 3;\n"
                         "    while (w < x) {\n"
                         "        if (y > z) then {\n"
                         "            call E;\n"
                         "        } else {\n"
                         "            print aa;\n"
                         "        }\n"
                         "    }\n"
                         "    bb = cc * 2;\n"
                         "}\n"
                         "\n"
                         "procedure D {\n"
                         "    read dd;\n"
                         "    if (ee != ff) then {\n"
                         "        print gg;\n"
                         "    } else {\n"
                         "        hh = ii + 5;\n"
                         "    }\n"
                         "    while (jj > kk) {\n"
                         "        print ll;\n"
                         "    }\n"
                         "}\n"
                         "\n"
                         "procedure E {\n"
                         "    mm = nn - 3;\n"
                         "    print oo;\n"
                         "    while (pp < qq) {\n"
                         "        a = b + 4;\n"
                         "    }\n"
                         "    rr = ss + 4;\n"
                         "}";
  std::string query = "stmt s;\n"
                      "Select s such that Modifies(s, \"m\")";

  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  WriteFacade writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);

    sourceProcessor.processSource(filename);
  ReadFacade readFacade = ReadFacade(*pkb_ptr);
  QPS qps(readFacade);

  std::unordered_set<std::string> results = qps.Evaluate(query);

  REQUIRE(results == std::unordered_set<std::string>({"i", "x", "y", "z"}));
}
