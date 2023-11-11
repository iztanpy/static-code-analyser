#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <stack>
#include <set>

// Headers from "SP" subdirectory
#include "SP/TNode.h"
#include "SP/sp_tokeniser/Token.h"
#include "Parser.h"
#include "SP/utils/ParseUtils.h"

// Headers from other directories
#include "PKB/PKB.h"
#include "utils/Error.h"

class DesignExtractor;
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
  void convertVarToEntity(Token* token);

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
  int parse(std::vector<Token>& tokens) override;
};
