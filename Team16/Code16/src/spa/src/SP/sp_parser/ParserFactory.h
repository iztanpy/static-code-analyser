#pragma once

// Include necessary headers
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#pragma once
#include <utility>

// Headers from "SP" subdirectory
#include "SP/TNode.h"
#include "SP/sp_tokeniser/Token.h"
#include "SP/sp_tokeniser/TokenTypes.h"
#include "SP/DesignExtractor.h"
#include "SP/sp_parser/SimpleParser.h"
#include "SP/sp_parser/AssignmentParser.h"
#include "SP/sp_parser/ReadParser.h"
#include "SP/sp_parser/ProcedureParser.h"
#include "SP/sp_parser/Parser.h"


#include "utils/Error.h"


/**
 * @class ParserFactory
 * @brief A factory class for creating and managing parsers for different statement types.
 *
 * The `ParserFactory` class is responsible for creating and managing various parsers used to
 * parse different types of statements represented as vectors of tokens. It provides methods to
 * parse statements and retrieve parsed results.
 */
class ParserFactory {
 private:
    ASTVisitor* visitor;
    int lineNumber = 1;
    AssignmentParser* assignmentParser;
    ReadParser* readParser;

 public:
    // Constructor for the factory class
    ParserFactory() : assignmentParser(nullptr), readParser(nullptr) {}

    /**
     * Parses a list of tokens and returns a vector of integers and strings.
     *
     * @param tokens A vector of tokens to be parsed.
     * @return A vector containing parsed integers and strings.
     */
    std::pair<int, std::string> parseStatements(std::vector<Token> tokens, int curr_index);
};
