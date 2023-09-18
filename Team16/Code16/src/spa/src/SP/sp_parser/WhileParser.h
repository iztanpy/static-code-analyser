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
#include "SP/sp_parser/SimpleParser.h"
#include "SP/sp_parser/ParserFactory.h"

// Headers from other directories
#include "PKB/PKB.h"
#include "utils/Error.h"

class SimpleParser;
/**
 * @class ReadeParser
 * @brief A concrete subclass of Parser specialized for parsing read statements.
 *
 */
class WhileParser : public Parser {
 public:
    explicit WhileParser(ASTVisitor* visitor) : visitor(visitor) {}
    int parse(const std::vector<Token>& tokens, int curr_index) override;
    std::vector<Token> getConditionTokens(const std::vector<Token>& tokens, int& curr_index);
    ASTVisitor* visitor;
    int lineNumber = 0;
    ParserFactory factory;
    /*std::unordered_map<std::string, std::unordered_set<std::string>> getModifiesVarHashmap();*/
};

