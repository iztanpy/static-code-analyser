#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <memory>
#include <stack>

// Headers from "SP" subdirectory
#include "SP/sp_parser/ParserFactory.h"

// Headers from other directories
#include "PKB/PKB.h"
#include "PKB/API/WriteFacade.h"
#include "utils/Error.h"

class DesignExtractor;  // Forward declaration
class AssignmentParser;
class WhileParser;
class ParserFactory;
class Parser;

/**
 * @class SimpleParser
 * @brief A concrete subclass of Parser for a simplified parsing task.
 *
 * The `SimpleParser` class inherits from the `Parser` class and provides an implementation for a simplified
 * parsing task. It includes an instance of `AssignmentParser` for parsing assignment statements.
 */
class SimpleParser : public Parser {
 private:
  ASTVisitor* visitor;
  ParserFactory* factory = new ParserFactory(visitor);
  Parser* parser;
  void checkCalls();

 public:
  explicit SimpleParser(ASTVisitor* visitor);
  int parse(std::vector<Token>& tokens) override;
  std::shared_ptr<TNode> rootTNode = nullptr;
};
