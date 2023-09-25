#include "ProcedureParser.h"

int ProcedureParser::parse(std::vector<Token>& tokens, int curr_index) {
    // validate procedure declaration: procedure (already validated), name, open brace
    if (curr_index + 2 > tokens.size()) {
        return -1;
    }
    // validate procedure name
    Token procedureNameToken = tokens[curr_index + 1];
    // check if name is keyword
    std::unordered_set<TokenType> keywords = { TokenType::kEntityAssign, TokenType::kEntityProcedure,
                                               TokenType::kEntityRead, TokenType::kEntityPrint,
                                               TokenType::kEntityWhile, TokenType::kEntityIf,
                                               TokenType::kEntityElse, TokenType::kEntityCall,
                                               TokenType::kEntityStmt, TokenType::kEntityConstant,
                                               TokenType::kEntityVariable };

    if (keywords.find(procedureNameToken.tokenType) != keywords.end()) {
        // set token to literal
        procedureNameToken.tokenType = TokenType::kLiteralName;
    }
    // check that name is literal
    if (procedureNameToken.tokenType != TokenType::kLiteralName) {
        return -1;
    }

    // validate procedure open brace
    size_t openBracesIndex = curr_index + 2;
    if (tokens[openBracesIndex].tokenType != TokenType::kSepOpenBrace) {
        return -1;
    }

    // build procedure ast
    Token procedure = tokens[curr_index];
    procedure.value = procedureNameToken.value;
    std::shared_ptr<TNode> root = TNodeFactory::createNode(procedure, 0);

    // set root node
    designExtractor->extractDesign(root, visitor);


    curr_index = curr_index + 3;
    return curr_index;
}
