#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <memory>
#include <stack>

// Headers from "SP" subdirectory
#include "SP/sp_parser/parser_factory.h"

// Headers from other directories
#include "PKB/pkb.h"
#include "PKB/API/write_facade.h"
#include "utils/error.h"

class design_extractor;  // Forward declaration
class assignment_parser;
class while_parser;
class ParserFactory;
class Parser;

/**
 * @class simple_parser
 * @brief A concrete subclass of Parser for a simplified parsing task.
 *
 * The `simple_parser` class inherits from the `Parser` class and provides an implementation for a simplified
 * parsing task. It includes an instance of `assignment_parser` for parsing assignment statements.
 */
class simple_parser : public Parser {
 private:
  ASTVisitor* visitor;
  ParserFactory* factory = new ParserFactory(visitor);
  Parser* parser;
  void checkCalls();

 public:
  explicit simple_parser(ASTVisitor* visitor);
  int parse(std::vector<Token>& tokens) override;
  std::shared_ptr<TNode> rootTNode = nullptr;
};
