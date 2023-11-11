#pragma once

// Include necessary headers
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <utility>

// Headers from "SP" subdirectory
#include "SP/sp_tokeniser/TokenTypes.h"
#include "SP/sp_parser/AssignmentParser.h"
#include "SP/sp_parser/ReadParser.h"
#include "SP/sp_parser/ProcedureParser.h"
#include "SP/sp_parser/Parser.h"
#include "SP/sp_parser/WhileParser.h"
#include "SP/sp_parser/IfParser.h"
#include "SP/sp_parser/PrintParser.h"
#include "SP/sp_parser/CallParser.h"
#include "SP/sp_parser/ElseParser.h"
#include "SP/sp_parser/CloseBraceParser.h"

// Headers from other directories
#include "utils/Error.h"

class Parser;
class ASTVisitor;

typedef std::unordered_map<TokenType, Parser*> ParserMap;

/**
 * @class ParserFactory
 * @brief A factory class for creating and managing parsers for different statement types.
 *
 * The `ParserFactory` class is responsible for creating and managing various parsers used to
 * parse different types of statements represented as vectors of tokens. It provides methods to
 * parse statements and retrieve parsed results.
 */
class ParserFactory {
 protected:
  ASTVisitor* visitor;

 public:
  explicit ParserFactory(ASTVisitor* visitor);

  /**
 * Create a parser for a given statement type.
 *
 * @return A pointer to a parser for the given statement type.
 */
  Parser* createParser(TokenType type, int lineNumber, int curr_index);

 private:
  ParserMap parserMap;
  AssignmentParser* assignmentParser = new AssignmentParser(visitor);
  ProcedureParser* procedureParser = new ProcedureParser(visitor);
  ReadParser* readParser = new ReadParser(visitor);
  WhileParser* whileParser = new WhileParser(visitor);
  PrintParser* printParser = new PrintParser(visitor);
  IfParser* ifParser = new IfParser(visitor);
  ElseParser* elseParser = new ElseParser(visitor);
  CallParser* callParser = new CallParser(visitor);
  CloseBraceParser* closeBraceParser = new CloseBraceParser(visitor);
  void setUpParserMap();
};
