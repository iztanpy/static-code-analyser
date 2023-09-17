#pragma once

#include <vector>
#include <string>
#include "TNode.h"
#include "Token.h"
#include "DesignExtractor.h"
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "PKB/PKB.h"
#include "PKB/API/WriteFacade.h"
#include "SPTokeniser.h"
#include "utils/Error.h"

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
    virtual int parse(const std::vector<Token>& tokens, int curr_index) = 0;
    DesignExtractor* designExtractor = new DesignExtractor();
};

/**
 * @class AssignmentParser
 * @brief A concrete subclass of Parser specialized for parsing assignment statements.
 *
 * The `AssignmentParser` class inherits from the `Parser` class and provides an implementation for parsing
 * assignment statements. It also contains methods for accessing information related to the parsed assignments.
 */
class AssignmentParser : public Parser {
 public:
    explicit AssignmentParser(ASTVisitor* visitor) : visitor(visitor) {}
    int parse(const std::vector<Token>& tokens, int curr_index) override;
    ASTVisitor* visitor;  // Initialize to nullptr in the constructor
    int lineNumber = 0;
};


/**
 * @class ProcedureParser
 * @brief A concrete subclass of Parser specialized for parsing assignment statements.
 *
 * The `ProcedureParser` class inherits from the `Parser` class and provides an implementation for parsing
 * assignment statements. It also contains methods for accessing information related to the parsed assignments.
 */
class ProcedureParser : public Parser {
 private:
    std::shared_ptr<TNode> rootTNode;
 public:
    explicit ProcedureParser(std::shared_ptr<TNode> rootTNode);
    int parse(const std::vector<Token>& tokens, int curr_index) override;
    ASTVisitor* procedureVisitor = new ASTVisitor();
    std::unordered_map<std::string, std::unordered_set<int>> getProcedureStatementNumberHashmap();
};

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
};
