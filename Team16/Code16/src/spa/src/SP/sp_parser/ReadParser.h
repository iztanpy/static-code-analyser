#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>

// Headers from "SP" subdirectory
#include "SP/TNode.h"
#include "SP/sp_tokeniser/Token.h"
#include "Parser.h"

// Headers from other directories
#include "PKB/PKB.h"
#include "utils/Error.h"

class DesignExtractor;

/**
 * @class ReadParser
 * @brief A class for parsing and processing read statements in SIMPLE program.
 *
 * The `ReadParser` class is responsible for parsing and processing read statements represented
 * as a vector of tokens. It extends the base `Parser` class and provides methods for parsing
 * and handling read statements.
 */
class ReadParser : public Parser {
 private:
  ASTVisitor* visitor;

 public:
  /**
   * @brief Constructs a ReadParser object with an associated ASTVisitor.
   * @param visitor A pointer to an ASTVisitor used for visiting nodes in the abstract syntax tree.
   */
  explicit ReadParser(ASTVisitor* visitor) : visitor(visitor), Parser() {}
  /**
  * @brief Parses a read statement starting from the given index in the token vector.
  *
  * This method is responsible for parsing a read statement from a vector of tokens starting
  * at the specified index. It processes the read statement and its associated content.
  *
  * @param tokens The vector of tokens representing the input code.
  * @param curr_index The current index in the token vector where parsing should start.
  * @return The index in the token vector after parsing the read statement.
  */
  int parse(std::vector<Token>& tokens) override;
};

