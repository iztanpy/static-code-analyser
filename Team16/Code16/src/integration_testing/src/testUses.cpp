//
// Created by Isaac Tan on 1/11/23.
//

//
// Created by Isaac Tan on 13/10/23.
//

#include "catch.hpp"
#include <memory>
#include <unordered_set>
#include <string>

#include "utils/hash_utils.h"
#include "SP/SourceProcessor.h"
#include "PKB/API/WriteFacade.h"
#include "PKB/API/ReadFacade.h"
#include "qps/qps.h"

TEST_CASE("TEST Simple Uses") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    std::string simpleProgram = "procedure a {\n"
                                "  x = 2;\n"
                                "  x = x + 1;\n"
                                "}\n"
                                "\n"
                                "procedure b {\n"
                                "  x = 2;\n"
                                "}\n"
                                "\n"
                                "procedure c {\n"
                                "  read x;\n"
                                "}\n"
                                "\n"
                                "procedure d {\n"
                                "  call a;\n"
                                "  call b;\n"
                                "}";

    sourceProcessor.processSource(simpleProgram);

    std::string query = "procedure p;\n"
                        "Select p such that Uses(p,_)";

    std::unordered_set<std::string> results = qps.Evaluate(query);

    REQUIRE(results == std::unordered_set<std::string>({"a", "d"}));
}