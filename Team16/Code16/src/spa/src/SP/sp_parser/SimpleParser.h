#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>

// Headers from "SP" subdirectory
#include "SP/sp_parser/AssignmentParser.h"
#include "SP/sp_parser/ReadParser.h"
#include "SP/sp_parser/ProcedureParser.h"
#include "SP/sp_parser/Parser.h"
#include "SP/sp_parser/WhileParser.h"
#include "SP/sp_parser/IfParser.h"
#include "SP/sp_parser/PrintParser.h"

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
  * parsing task. It includes an instance of `AssignmentParser` for parsing assignment statements.
  */
class SimpleParser : public Parser {
 private:
    WriteFacade* writeFacade;
    ASTVisitor* visitor;
    int lineNumber = 1;
 public:
    explicit SimpleParser(WriteFacade* writeFacade, ASTVisitor* visitor);
    int parse(const std::vector<Token>& tokens, int curr_index) override;
    std::shared_ptr<TNode> rootTNode = nullptr;
    AssignmentParser* assignmentParser = new AssignmentParser(visitor);
    ProcedureParser* procedureParser = new ProcedureParser(rootTNode);
    ReadParser* readParser = new ReadParser(visitor);
    WhileParser* whileParser = new WhileParser(visitor);
    PrintParser* printParser = new PrintParser(visitor);
    IfParser* ifParser = new IfParser(visitor);
};

