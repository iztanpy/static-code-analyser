#include "CallParser.h"

int CallParser::parse(const std::vector<Token>& tokens, int curr_index) {
    // Check if there are enough tokens for a valid call statement
    if (curr_index + 3 > tokens.size()) {
        return -1;
    }

    // Validate call name
    Token callNameToken = tokens[curr_index + 1];
    Token semicolonToken = tokens[curr_index + 2];

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
    Token call = tokens[curr_index];
    call.value = callNameToken.value;

    // Update the current index and create the AST node
    curr_index = curr_index + 3;
    std::shared_ptr<TNode> root = TNodeFactory::createNode(call, lineNumber);
    return curr_index;
}

