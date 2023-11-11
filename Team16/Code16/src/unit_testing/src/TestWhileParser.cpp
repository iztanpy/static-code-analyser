#include "catch.hpp"
#include "SP/sp_tokeniser/token.h"
#include "SP/source_processor.h"
#include "PKB/API/write_facade.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <memory>


//TEST_CASE(("Test Conditional Tokens Retrieval")) {
//    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//    auto writeFacade = write_facade(*pkb_ptr);
//    SourceProcessor sourceProcessor(&writeFacade);
//    std::string simpleProgram2 = "procedure p { while (x == 1) { read x; } }";
//    sourceProcessor.processSource(simpleProgram2);
//
//    REQUIRE(1 == 1);
//}