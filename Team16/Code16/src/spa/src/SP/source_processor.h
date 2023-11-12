#pragma once

#include <vector>
#include <utility>
#include <set>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "SP/sp_parser/parser.h"
#include "SP/sp_parser/simple_parser.h"
#include "SP/tnode.h"
#include "SP/sp_tokeniser/token.h"
#include "SP/design_extractor.h"
#include "PKB/pkb.h"
#include "PKB/API/write_facade.h"
#include "SP/sp_tokeniser/sp_tokeniser.h"
#include "utils/error.h"

/**
 * @class SourceProcessor
 * @brief An abstraction responsible for processing and analyzing source code in the Simple Programming Language (SPL).
 *
 * The `SourceProcessor` class is responsible for processing and analyzing source code written in the
 * Simple Programming Language (SPL). It performs tokenization, parsing, and analysis tasks to extract
 * information about the program's structure and behavior by accessing methods from the SimpleParser.
 */
class SourceProcessor {
private:
  std::unique_ptr<PKB> pkb_ptr;
  SPtokeniser tokeniser;
  simple_parser simpleParser;
  ASTVisitor visitor;
  WriteFacade* writeFacade;

public:
  /**
   * @brief Constructs a SourceProcessor object with a WriteFacade for writing analysis results.
   * @param writeFacade A pointer to the WriteFacade used for writing analysis results.
   */
  explicit SourceProcessor(WriteFacade* writeFacade);
  /**
  * @brief Processes the source code and performs analysis.
  * @param simpleProgram The Simple Programming Language (SPL) source code to be processed.
   */
  void processSource(const std::string& simpleProgram);
  /**
   * @brief Retrieves the visitor.
   * @return The ASTVisitor object.
   */
  ASTVisitor getVisitor() const;
};

