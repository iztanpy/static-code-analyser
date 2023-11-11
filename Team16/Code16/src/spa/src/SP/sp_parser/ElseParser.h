#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <set>

// Headers from "SP" subdirectory
#include "SP/utils/ParseUtils.h"
#include "SP/TNode.h"
#include "SP/sp_tokeniser/Token.h"
#include "Parser.h"

// Headers from other directories
#include "PKB/pkb.h"
#include "utils/Error.h"

class DesignExtractor;

class ElseParser : public Parser {
 private:
  ASTVisitor* visitor;

 public:
  /**
   * @brief Constructs an ElseParser object with an associated ASTVisitor.
   * @param visitor A pointer to an ASTVisitor used for visiting nodes in the abstract syntax tree.
  */
  explicit ElseParser(ASTVisitor* visitor) : visitor(visitor) {}
  /**
    * @brief Parses an "else" statement starting from the given index in the token vector.
    *
    * This method is responsible for parsing an "else" statement from a vector of tokens starting at
    * the specified index. It processes the "else" condition and associated code block.
    *
    * @param tokens The vector of tokens representing the input code.
    * @param curr_index The current index in the token vector where parsing should start.
    * @return The index in the token vector after parsing the "else" statement.
  */
  int parse(std::vector<Token>& tokens) override;
};

