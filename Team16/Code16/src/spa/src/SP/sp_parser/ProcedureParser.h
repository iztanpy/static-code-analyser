#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <memory>

// Headers from "SP" subdirectory
#include "SP/TNode.h"
#include "SP/sp_tokeniser/Token.h"
#include "Parser.h"
#include "SP/utils/ParseUtils.h"
#include "SP/sp_cfg/CfgNode.h"

// Headers from other directories
#include "PKB/PKB.h"
#include "utils/Error.h"

class DesignExtractor;
/**
 * @class ProcedureParser
 * @brief A concrete subclass of Parser specialized for parsing procedure statements.
 *
 * The `ProcedureParser` class inherits from the `Parser` class and provides an implementation for parsing
 * procedure statements. It also contains methods for accessing information related to the parsed procedures.
 */
class ProcedureParser : public Parser {
 public:
    /**
    * @brief Constructs a ProcedureParser object with an associated ASTVisitor.
    * @param visitor A pointer to an ASTVisitor used for visiting nodes in the abstract syntax tree.
    */
    explicit ProcedureParser(ASTVisitor* visitor) : visitor(visitor) {}
    /**
     * @brief Parses a procedure statement starting from the given index in the token vector.
     *
     * This method is responsible for parsing a procedure statement from a vector of tokens starting
     * at the specified index. It processes the procedure statement and its associated content.
     *
     * @param tokens The vector of tokens representing the input code.
     * @param curr_index The current index in the token vector where parsing should start.
     * @return The index in the token vector after parsing the procedure statement.
     */
    int parse(std::vector<Token>& tokens) override;
    static void addCfgNodeToMap(const string& procedureName, std::shared_ptr<CfgNode> cfgNode);
    ASTVisitor* visitor;
};

