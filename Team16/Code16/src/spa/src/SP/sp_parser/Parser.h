#pragma once

#include <vector>
#include <stack>
#include <set>
#include <string>
#include <unordered_set>
#include <unordered_map>

// Headers from "SP" subdirectory
#include "SP/sp_tokeniser/Token.h"
#include "SP/DesignExtractor.h"
#include "SP/sp_cfg/Cfg.h"
#include "SP/sp_cfg/CfgNode.h"

// Headers from other directories
#include "utils/Error.h"

class DesignExtractor;  // Forward declaration
class AssignmentParser;
class ReadParser;
class ProcedureParser;
class WhileParser;
class IfParser;
class ElseParser;
class PrintParser;
class CallParser;
class CloseBraceParser;

/**
 * @class Parser
 * @brief Abstract base class for parsing operations on a sequence of tokens.
 *
 * The `Parser` class defines the common interface for parsing operations on a sequence of tokens.
 * Subclasses of `Parser` are expected to provide specific implementations for parsing different parts of the input.
 *
 * @note This class is designed to be abstract and cannot be instantiated directly. Subclasses must override the
 *       `parse` method to perform custom parsing logic.
 **/
class Parser {
 public:
    Parser() = default;
    virtual ~Parser() = default;
    virtual int parse(std::vector<Token>& tokens) = 0;
    void start_parse(std::vector<Token>& tokens, int curr_index);
    static DesignExtractor* designExtractor;  // Initialize to nullptr in the constructor
    static int index;
    static Cfg cfgFacade;
    std::shared_ptr<CfgNode> rootCfgNode = std::make_shared<CfgNode>(0);

    /**
    * @brief Increment the index used for token parsing.
    *
    * This method increments the index used for token parsing within the vector of tokens.
    * It is typically used after successfully parsing a token or a portion of the input.
    */
    void incrementIndex() {
      index++;
    }
    /**
     * @brief Retrieves the current line number.
     * @return The current line number.
    */
    int getLineNumber() {
      return lineNumber;
    }
    /**
     * @brief Sets the line number to the specified value.
     * @param newLineNumber The new line number to set.
     */
    void setLineNumber(int newLineNumber) {
      lineNumber = newLineNumber;
    }
    /**
    * @brief Retrieves the current index.
    * @return The current index.
    */
    int getIndex() {
      return index;
    }
    /**
     * @brief Sets the index to the specified value.
     * @param newIndex The new index to set.
     */
    void setIndex(int newIndex) {
      index = newIndex;
    }

    void reset() {
        index = 0;
        lineNumber = 1;
        currWhileDepth = 0;
        currIfDepth = 0;
        isParsingProcedure = false;
        followsStatementStack = std::stack<std::set<int>>();
        controlStructureStack = std::stack<std::string>();
        parentStatementNumberHashmap = std::unordered_map<int, std::unordered_set<int>>();
        parentStatementStack = std::stack<int>();
    }

 protected:
  static std::stack<std::set<int>> followsStatementStack;
  static std::stack<std::string> controlStructureStack;  // Track the current control structures (if, else, while)
  static std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap;
  static std::stack<int> parentStatementStack;  // Track the parent statement lines
  static int lineNumber;
  static int currWhileDepth;
  static int currIfDepth;
  static bool isParsingProcedure;
};
