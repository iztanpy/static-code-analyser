#include "ReadParser.h"

int ReadParser::parse(std::vector<Token>& tokens, int curr_index) {
    // Check if there are enough tokens for a valid read statement
    if (curr_index + 3 > tokens.size()) {
        return -1;
    }

    // Validate read name
    Token readNameToken = tokens[curr_index + 1];
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

    // Check if the read name token is a keyword and convert it to a literal if necessary
    if (validKeywords.find(readNameToken.tokenType) != validKeywords.end()) {
        readNameToken.tokenType = TokenType::kLiteralName;
    }

    // Check if the read name is a literal
    if (readNameToken.tokenType != TokenType::kLiteralName) {
        return -1;
    }

    // Check if the read statement is terminated correctly
    if (semicolonToken.tokenType != TokenType::kSepSemicolon) {
        return -1;
    }

    // Update the value of the 'read' token to match the read name
    Token read = tokens[curr_index];
    read.value = readNameToken.value;

    // Update the current index and create the AST node
    curr_index = curr_index + 3;
    std::shared_ptr<TNode> root = TNodeFactory::createNode(read, lineNumber);
    designExtractor->extractDesign(root, visitor);
    return curr_index;
}
