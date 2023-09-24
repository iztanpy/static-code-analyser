#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <memory>
#include <stack>

// Headers from "SP" subdirectory
#include "SP/sp_parser/AssignmentParser.h"
#include "SP/sp_parser/ReadParser.h"
#include "SP/sp_parser/ProcedureParser.h"
#include "SP/sp_parser/Parser.h"
#include "SP/sp_parser/WhileParser.h"
#include "SP/sp_parser/IfParser.h"
#include "SP/sp_parser/PrintParser.h"
#include "SP/sp_parser/CallParser.h"

// Headers from other directories
#include "PKB/PKB.h"
#include "PKB/API/WriteFacade.h"
#include "utils/Error.h"


class DesignExtractor;  // Forward declaration
class AssignmentParser;
class WhileParser;

/**
 * @class SimpleParser
 * @brief A concrete subclass of Parser for a simplified parsing task.
 *
 * The `SimpleParser` class inherits from the `Parser` class and provides an implementation for a simplified
 * parsing task. It includes an instance of `AssignmentParser` for parsing assignment statements and other parsers
 * for different statement types.
 */
class SimpleParser : public Parser {
 private:
    WriteFacade* writeFacade;
    ASTVisitor* visitor;
    int lineNumber = 1;
    int nestingLevel = 0;
    int currWhileDepth;
    int currIfDepth;
    bool isParsingProcedure;
 public:
     /**
     * @brief Constructs a SimpleParser object with an associated WriteFacade and ASTVisitor.
     *
     * The constructor initializes the parser with a WriteFacade for writing analysis results
     * and an ASTVisitor for visiting nodes in the abstract syntax tree.
     *
     * @param writeFacade A pointer to the WriteFacade used for writing analysis results.
     * @param visitor A pointer to the ASTVisitor used for visiting nodes in the abstract syntax tree.
     */
    explicit SimpleParser(WriteFacade* writeFacade, ASTVisitor* visitor);
    /**
    * @brief Parses a list of tokens representing code starting from the given index.
    *
    * This method is responsible for parsing a list of tokens representing code starting at the specified index.
    * It processes different types of statements using the associated parsers.
    *
    * @param tokens The vector of tokens representing the input code.
    * @param curr_index The current index in the token vector where parsing should start.
    * @return The index in the token vector after parsing the code.
    */
    int parse(std::vector<Token>& tokens, int curr_index) override;
    std::shared_ptr<TNode> rootTNode = nullptr;
    AssignmentParser* assignmentParser = new AssignmentParser(visitor);
    ProcedureParser* procedureParser = new ProcedureParser(visitor);
    ReadParser* readParser = new ReadParser(visitor);
    WhileParser* whileParser = new WhileParser(visitor);
    PrintParser* printParser = new PrintParser(visitor);
    IfParser* ifParser = new IfParser(visitor);
    CallParser* callParser = new CallParser(visitor);
    /**
     * @brief Inserts a set of follow relationship data into the appropriate data structure.
     *
     * This method inserts a set of follow relationship data into the appropriate data structure
     * for further analysis and processing.
     *
     * @param followsSet A set of integers representing follow relationship data.
     */
    void insertFollowsHashMap(std::set<int> followsSet);
};

