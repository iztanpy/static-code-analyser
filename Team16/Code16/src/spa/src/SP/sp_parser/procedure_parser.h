#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <memory>

// Headers from "SP" subdirectory
#include "SP/tnode.h"
#include "SP/sp_tokeniser/token.h"
#include "SP/sp_parser/parser.h"
#include "SP/utils/parse_utils.h"
#include "SP/sp_cfg/cfg_node.h"

// Headers from other directories
#include "PKB/pkb.h"
#include "utils/error.h"

class design_extractor;
/**
 * @class ProcedureParser
 * @brief A concrete subclass of Parser specialized for parsing procedure statements.
 *
 * The `ProcedureParser` class inherits from the `Parser` class and provides an implementation for parsing
 * procedure statements. It also contains methods for accessing information related to the parsed procedures.
 */
class ProcedureParser : public Parser {
 private:
  ASTVisitor* visitor;

 public:
  /**
  * @brief Constructs a ProcedureParser object with an associated ASTVisitor.
  * @param visitor A pointer to an ASTVisitor used for visiting nodes in the abstract syntax tree.
  */
  explicit ProcedureParser(ASTVisitor* visitor) : visitor(visitor) {}
  /**
   * @brief Parses a procedure statement starting from the given index in the token vector.
   *
   * This method is responsible for parsing a procedure statement from a vector of tokens starting
   * at the specified index. It processes the procedure statement and its associated content.
   *
   * @param tokens The vector of tokens representing the input code.
   * @param curr_index The current index in the token vector where parsing should start.
   * @return The index in the token vector after parsing the procedure statement.
   */
  int parse(std::vector<Token>& tokens) override;
};

