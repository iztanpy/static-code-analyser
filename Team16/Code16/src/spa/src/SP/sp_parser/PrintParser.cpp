#include "PrintParser.h"

int PrintParser::parse(const std::vector<Token>& tokens, int curr_index) {
    // Check if there are enough tokens for a valid print statement
    if (curr_index + 3 > tokens.size()) {
        return -1;
    }

    Token printNameToken = tokens[curr_index + 1];
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

    // Check if the procedure name token is a keyword and convert it to a literal if necessary
    if (validKeywords.find(printNameToken.tokenType) != validKeywords.end()) {
        printNameToken.tokenType = TokenType::kLiteralName;
    }

    // Check if the procedure name is a literal
    if (printNameToken.tokenType != TokenType::kLiteralName) {
        return -1;
    }

    // Check if the print statement is terminated correctly
    if (semicolonToken.tokenType != TokenType::kSepSemicolon) {
        return -1;
    }

    // Update the value of the 'print' token to match the procedure name
    Token print = tokens[curr_index];
    print.value = printNameToken.value;

    // Update the current index and create the AST node
    curr_index = curr_index + 3;
    std::shared_ptr<TNode> root = TNodeFactory::createNode(print, lineNumber);
    return curr_index;
}

