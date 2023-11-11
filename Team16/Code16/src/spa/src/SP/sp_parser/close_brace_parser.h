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

class close_brace_parser : public Parser {
 private:
  ASTVisitor* visitor;
  void insertFollowsHashMap(std::set<int> followsSet);

 public:
  /**
   * @brief Constructs an close_brace_parser object with an associated ASTVisitor.
   * @param visitor A pointer to an ASTVisitor used for visiting nodes in the abstract syntax tree.
  */
  explicit close_brace_parser(ASTVisitor* visitor) : visitor(visitor) {}
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
