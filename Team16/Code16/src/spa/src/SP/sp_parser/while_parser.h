#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <set>

// Headers from "SP" subdirectory
#include "SP/sp_parser/parser.h"
#include "SP/utils/parse_utils.h"
#include "SP/tnode.h"
#include "SP/sp_tokeniser/token.h"

// Headers from other directories
#include "PKB/pkb.h"
#include "utils/error.h"

class design_extractor;
/**
 * @class while_parser
 * @brief A concrete subclass of Parser specialized for parsing while statements.
 *
 * The `while_parser` class is responsible for parsing while statements represented as a vector of tokens.
 * It extends the base `Parser` class and provides methods for parsing and handling while statements.
 */
class while_parser : public Parser {
 private:
  ASTVisitor* visitor;

 public:
  /**
  * @brief Constructs a while_parser object with an associated ASTVisitor.
  * @param visitor A pointer to an ASTVisitor used for visiting nodes in the abstract syntax tree.
  */
  explicit while_parser(ASTVisitor* visitor) : visitor(visitor) {}
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
  int parse(std::vector<Token>& tokens) override;
};

