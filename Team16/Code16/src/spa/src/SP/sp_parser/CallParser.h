#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>

// Headers from "SP" subdirectory
#include "SP/TNode.h"
#include "SP/sp_tokeniser/Token.h"
#include "SP/DesignExtractor.h"
#include "SP/sp_parser/Parser.h"

// Headers from other directories
#include "PKB/PKB.h"
#include "utils/Error.h"

class SimpleParser;
/**
 * @class CallParser
 * @brief A class for parsing and processing function call statements in the SIMPLE Program.
 *
 * The `CallParser` class is responsible for parsing and processing function call statements represented
 * as a vector of tokens. It extends the base `Parser` class and provides methods for parsing
 * and handling function call statements.
 */
class CallParser : public Parser {
 public:
    /**
     * @brief Constructs a CallParser object with an associated ASTVisitor.
     * @param visitor A pointer to an ASTVisitor used for visiting nodes in the abstract syntax tree.
    */
    explicit CallParser(ASTVisitor* visitor) : visitor(visitor) {}
    /**
     * @brief Parses a function call statement starting from the given index in the token vector.
     *
     * This method is responsible for parsing a function call statement from a vector of tokens starting
     * at the specified index. It processes the function call and its associated content.
     *
     * @param tokens The vector of tokens representing the input code.
     * @param curr_index The current index in the token vector where parsing should start.
     * @return The index in the token vector after parsing the function call statement.
     */
    int parse(std::vector<Token>& tokens, int curr_index) override;
    ASTVisitor* visitor;
    int lineNumber = 0;
};

