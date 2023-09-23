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

// Headers from other directories
#include "PKB/PKB.h"
#include "utils/Error.h"

class ConditionParser : public Parser {
 public:
    explicit ConditionParser(ASTVisitor* visitor) : visitor(visitor) {}
    int parse(std::vector<Token>& tokens, int curr_index) override;
    std::vector<Token> getConditionTokens(const std::vector<Token>& tokens, int curr_index);
    int evaluateCondition(const std::vector<Token>& tokens, int curr_index);
    int parseRelationalExpression(const std::vector<Token>& tokens, int curr_index);
    int parseFactor(const std::vector<Token>& tokens, int curr_index);
    int parseTerm(const std::vector<Token>& tokens, int curr_index);
    int parseRelFactor(const std::vector<Token>& tokens, int curr_index);
    int parseLogicalExpression(const std::vector<Token>& tokens, int curr_index);
    int parseConditionExpression(const std::vector<Token>& tokens, int curr_index);
    int parseNotExpression(const std::vector<Token>& tokens, int curr_index);
    bool isRelationalOperator(const Token& token);
    bool isLogicalOperator(const Token& token);

    ASTVisitor* visitor;
    int lineNumber = 0;
    /*std::unordered_map<std::string, std::unordered_set<std::string>> getModifiesVarHashmap();*/
};
