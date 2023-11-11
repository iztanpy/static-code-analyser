#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <set>

// Headers from "SP" subdirectory
#include "SP/utils/parse_utils.h"
#include "SP/tnode.h"
#include "SP/sp_tokeniser/token.h"
#include "SP/sp_parser/parser.h"

// Headers from other directories
#include "PKB/pkb.h"
#include "utils/error.h"

class design_extractor;
/**
 * @class if_parser
 * @brief A class for parsing "if" statements in the SIMPLE Program.
 *
 * This class extends the base `Parser` class and is specifically designed for parsing "if" statements
 * represented as a vector of tokens. It provides methods for parsing and processing "if" statements.
 */
class if_parser : public Parser {
 private:
  ASTVisitor* visitor;

 public:
  /**
   * @brief Constructs an if_parser object with an associated ASTVisitor.
   * @param visitor A pointer to an ASTVisitor used for visiting nodes in the abstract syntax tree.
  */
  explicit if_parser(ASTVisitor* visitor) : visitor(visitor) {}
  /**
    * @brief Parses an "if" statement starting from the given index in the token vector.
    *
    * This method is responsible for parsing an "if" statement from a vector of tokens starting at
    * the specified index. It processes the "if" condition and associated code block.
    *
    * @param tokens The vector of tokens representing the input code.
    * @param curr_index The current index in the token vector where parsing should start.
    * @return The index in the token vector after parsing the "if" statement.
  */
  int parse(std::vector<Token>& tokens) override;
};
