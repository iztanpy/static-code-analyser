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
