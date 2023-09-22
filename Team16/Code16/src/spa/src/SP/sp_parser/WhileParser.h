#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>

// Headers from "SP" subdirectory
#include "SP/TNode.h"
#include "SP/sp_tokeniser/Token.h"
#include "SP/sp_tokeniser/TokenTypes.h"
#include "SP/DesignExtractor.h"
#include "SP/sp_parser/Parser.h"
#include "SP/utils/ParseUtils.h"

// Headers from other directories
#include "PKB/PKB.h"
#include "utils/Error.h"

/**
 * @class WhileParser
 * @brief A concrete subclass of Parser specialized for parsing while statements.
 *
 */
class WhileParser : public Parser {
 public:
    explicit WhileParser(ASTVisitor* visitor) : visitor(visitor) {}
    int parse(const std::vector<Token>& tokens, int curr_index) override;
    ASTVisitor* visitor;
    int lineNumber = 0;
    int index = 0;
    /*std::unordered_map<std::string, std::unordered_set<std::string>> getModifiesVarHashmap();*/
};

