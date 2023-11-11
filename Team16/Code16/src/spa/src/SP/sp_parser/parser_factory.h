#pragma once

// Include necessary headers
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <utility>

// Headers from "SP" subdirectory
#include "SP/sp_tokeniser/token_types.h"
#include "SP/sp_parser/assignment_parser.h"
#include "SP/sp_parser/read_parser.h"
#include "SP/sp_parser/procedure_parser.h"
#include "SP/sp_parser/parser.h"
#include "SP/sp_parser/while_parser.h"
#include "SP/sp_parser/if_parser.h"
#include "SP/sp_parser/print_parser.h"
#include "SP/sp_parser/call_parser.h"
#include "SP/sp_parser/else_parser.h"
#include "SP/sp_parser/close_brace_parser.h"

// Headers from other directories
#include "utils/error.h"

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
  assignment_parser* assignmentParser = new assignment_parser(visitor);
  ProcedureParser* procedureParser = new ProcedureParser(visitor);
  read_parser* readParser = new read_parser(visitor);
  while_parser* whileParser = new while_parser(visitor);
  PrintParser* printParser = new PrintParser(visitor);
  if_parser* ifParser = new if_parser(visitor);
  ElseParser* elseParser = new ElseParser(visitor);
  call_parser* callParser = new call_parser(visitor);
  close_brace_parser* closeBraceParser = new close_brace_parser(visitor);
  void setUpParserMap();
};
