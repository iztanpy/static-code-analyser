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

class CloseBraceParser : public Parser {
 private:
  ASTVisitor* visitor;
  void insertFollowsHashMap(std::set<int> followsSet);

 public:
  /**
   * @brief Constructs an CloseBraceParser object with an associated ASTVisitor.
   * @param visitor A pointer to an ASTVisitor used for visiting nodes in the abstract syntax tree.
  */
  explicit CloseBraceParser(ASTVisitor* visitor) : visitor(visitor) {}
  /**
    * @brief Parses an "}" starting from the given index in the token vector.
    *
    * This method is responsible for parsing an "}" from a vector of tokens starting at
    * the specified index.
    *
    * @param tokens The vector of tokens representing the input code.
    * @param curr_index The current index in the token vector where parsing should start.
    * @return The index in the token vector after parsing "}".
  */
  int parse(std::vector<Token>& tokens) override;
};
