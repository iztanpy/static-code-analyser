#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>

// Headers from "SP" subdirectory
#include "SP/TNode.h"
#include "SP/sp_tokeniser/Token.h"
#include "SP/DesignExtractor.h"
#include "SP/sp_parser/Parser.h"

// Headers from other directories
#include "PKB/PKB.h"
#include "utils/Error.h"


/**
 * @class AssignmentParser
 * @brief A concrete subclass of Parser specialized for parsing assignment statements.
 *
 * The `AssignmentParser` class inherits from the `Parser` class and provides an implementation for parsing
 * assignment statements. It also contains methods for accessing information related to the parsed assignments.
 */
class AssignmentParser : public Parser {
 public:
    AssignmentParser() = default;
    int parse(const std::vector<Token>& tokens, int curr_index) override;
    ASTVisitor* visitor = new ASTVisitor();
    int lineNumber = 0;

    std::unordered_map<std::string, std::unordered_set<std::string>> getAssignVarHashmap();
    std::unordered_map<std::string, std::unordered_set<std::string>> getAssignConstHashmap();
    std::unordered_set<std::string> getVariablesHashset();
    std::unordered_set<std::string> getConstantsHashset();

    std::unordered_map<int, std::unordered_set<std::string>> getUsesStatementNumberHashmap();
    std::unordered_map<int, std::string> getUsesStatementNumberVarHashmap();
    std::unordered_set<int> getAssignmentStatementsHashset();
};
