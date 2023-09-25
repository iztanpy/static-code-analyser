#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>

// Headers from "SP" subdirectory
#include "SP/TNode.h"
#include "SP/sp_tokeniser/Token.h"
#include "SP/sp_tokeniser/TokenTypes.h"
#include "SP/DesignExtractor.h"
#include "SP/sp_parser/Parser.h"
#include "SP/utils/ParseUtils.h"

// Headers from other directories
#include "PKB/PKB.h"
#include "utils/Error.h"

/**
 * @class WhileParser
 * @brief A concrete subclass of Parser specialized for parsing while statements.
 *
 * The `WhileParser` class is responsible for parsing while statements represented as a vector of tokens.
 * It extends the base `Parser` class and provides methods for parsing and handling while statements.
 */
class WhileParser : public Parser {
 private:
    ASTVisitor* visitor;
    int lineNumber = 0;
    int index = 0;

 public:
     /**
     * @brief Constructs a WhileParser object with an associated ASTVisitor.
     * @param visitor A pointer to an ASTVisitor used for visiting nodes in the abstract syntax tree.
     */
    explicit WhileParser(ASTVisitor* visitor) : visitor(visitor) {}
    /**
    * @brief Parses a while statement starting from the given index in the token vector.
    *
    * This method is responsible for parsing a while statement from a vector of tokens starting at
    * the specified index. It processes the while statement and its associated content.
    *
    * @param tokens The vector of tokens representing the input code.
    * @param curr_index The current index in the token vector where parsing should start.
    * @return The index in the token vector after parsing the while statement.
    */
    int parse(std::vector<Token>& tokens, int curr_index) override;
    /**
     * @brief Retrieves the current line number.
     * @return The current line number.
    */
    int getLineNumber();
    /**
     * @brief Sets the line number to the specified value.
     * @param newLineNumber The new line number to set.
     */
    void setLineNumber(int newLineNumber);
    /**
    * @brief Retrieves the current index.
    * @return The current index.
    */
    int getIndex();
    /**
     * @brief Sets the index to the specified value.
     * @param newIndex The new index to set.
     */
    void setIndex(int newIndex);
};

