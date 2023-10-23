#include "CallParser.h"

int CallParser::parse(std::vector<Token>& tokens) {
    // Check if there are enough tokens for a valid call statement
    if (index + 3 > tokens.size()) {
        return -1;
    }

    // Validate call name
    Token callNameToken = tokens[index + 1];
    Token semicolonToken = tokens[index + 2];

    // Define the set of valid keyword token types
    std::unordered_set<TokenType> validKeywords = {
        TokenType::kEntityAssign, TokenType::kEntityProcedure,
        TokenType::kEntityRead, TokenType::kEntityPrint,
        TokenType::kEntityWhile, TokenType::kEntityIf,
        TokenType::kEntityElse, TokenType::kEntityCall,
        TokenType::kEntityStmt, TokenType::kEntityConstant,
        TokenType::kEntityVariable
    };

    // TODO(shaine): Perhaps the token type here should be a procedure because call calls upon a procedure
    if (validKeywords.find(callNameToken.tokenType) != validKeywords.end()) {
        callNameToken.tokenType = TokenType::kLiteralName;
    }

    // Check if the call name is a literal
    if (callNameToken.tokenType != TokenType::kLiteralName) {
        return -1;
    }

    // Check if the call statement is terminated correctly
    if (semicolonToken.tokenType != TokenType::kSepSemicolon) {
        return -1;
    }

    // Update the value of the 'call' token to match the call name
    Token call = tokens[index];
    call.value = callNameToken.value;

    std::string currentProcedureName = ParseUtils::getProcedureName();
    if (currentProcedureName == call.value) {  // procedure should not be able to call itself
        throw InvalidSemanticError();
    }
    visitor->setCallerCalleeMap(currentProcedureName, call.value);
    visitor->setCallStatementNumberEntityHashmap(lineNumber, call.value);
    // Update the current index and create the AST node
    index = index + 3;
    std::shared_ptr<TNode> root = TNodeFactory::createNode(call, lineNumber);
    Cfg::handleStatement(lineNumber);
    designExtractor->extractDesign(root, visitor);
    lineNumber++;
    return index;
}
