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
#include "SP/utils/ParseUtils.h"

// Headers from other directories
#include "PKB/PKB.h"
#include "utils/Error.h"


/**
 * @class AssignmentParser
 * @brief A concrete subclass of Parser specialized for parsing assignment statements.
 *
 * The `AssignmentParser` class inherits from the `Parser` class and provides an implementation for parsing
 * assignment statements. It also contains methods for accessing information related to the parsed assignments.
 */
class AssignmentParser : public Parser {
 public:
    explicit AssignmentParser(ASTVisitor* visitor) : visitor(visitor) {}
    int parse(std::vector<Token>& tokens, int curr_index) override;
    std::shared_ptr<TNode> parseExpression(const std::vector<Token>& tokens);
    std::shared_ptr<TNode> parseTerm(const std::vector<Token>& tokens);
    std::shared_ptr<TNode> parseFactor(const std::vector<Token>& tokens);
    void incrementIndex();

    ASTVisitor* visitor;  // Initialize to nullptr in the constructor
    int lineNumber = 0;
    int index = 0;
};

