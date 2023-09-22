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

class SimpleParser;
/**
 * @class CallParser
 * @brief A concrete subclass of Parser specialized for parsing call statements.
 *
 */
class CallParser : public Parser {
 public:
    explicit CallParser(ASTVisitor* visitor) : visitor(visitor) {}
    int parse(std::vector<Token>& tokens, int curr_index) override;
    ASTVisitor* visitor;
    int lineNumber = 0;
    /*std::unordered_map<std::string, std::unordered_set<std::string>> getModifiesVarHashmap();*/
};

