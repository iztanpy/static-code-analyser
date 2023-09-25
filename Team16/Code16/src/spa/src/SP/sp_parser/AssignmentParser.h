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
 private:
    ASTVisitor* visitor;  // Initialize to nullptr in the constructor
    int lineNumber = 0;
    int index = 0;

 public:
    /**
     * @brief Constructor for the AssignmentParser class.
     *
     * @param visitor A pointer to an ASTVisitor instance used for parsing and visiting the abstract syntax tree (AST).
    */
    explicit AssignmentParser(ASTVisitor* visitor) : visitor(visitor) {}
    /**
     * @brief Parse assignment statements from a vector of tokens.
     *
     * This method parses assignment statements from a vector of tokens starting from the specified `curr_index`.
     * It updates the `curr_index` to point to the next token after parsing assignments.
     *
     * @param tokens A vector of tokens to parse.
     * @param curr_index The current index in the vector of tokens to start parsing from.
     * @return The index of the next token to be parsed after the assignment statements.
     */
    int parse(std::vector<Token>& tokens, int curr_index) override;
    /**
     * @brief Parse an expression from a vector of tokens.
     *
     * This method parses an expression from a vector of tokens and returns the corresponding abstract syntax tree (AST) node.
     *
     * @param tokens A vector of tokens representing the expression to be parsed.
     * @return A shared pointer to the root AST node of the parsed expression.
     */
    std::shared_ptr<TNode> parseExpression(const std::vector<Token>& tokens);
    /**
     * @brief Parse a term from a vector of tokens.
     *
     * This method parses a term from a vector of tokens and returns the corresponding abstract syntax tree (AST) node.
     *
     * @param tokens A vector of tokens representing the term to be parsed.
     * @return A shared pointer to the root AST node of the parsed term.
     */
    std::shared_ptr<TNode> parseTerm(const std::vector<Token>& tokens);
    /**
     * @brief Parse a factor from a vector of tokens.
     *
     * This method parses a factor from a vector of tokens and returns the corresponding abstract syntax tree (AST) node.
     *
     * @param tokens A vector of tokens representing the factor to be parsed.
     * @return A shared pointer to the root AST node of the parsed factor.
     */
    std::shared_ptr<TNode> parseFactor(const std::vector<Token>& tokens);
    /**
     * @brief Increment the index used for token parsing.
     *
     * This method increments the index used for token parsing within the vector of tokens.
     * It is typically used after successfully parsing a token or a portion of the input.
     */
    void incrementIndex();
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

