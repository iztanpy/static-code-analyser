#include <iostream>
#include <vector>
#include "SimpleParser.h"

ProcedureParser::ProcedureParser(std::shared_ptr<TNode> rootTNode) : rootTNode(rootTNode) { }

int ProcedureParser::parse(const std::vector<Token>& tokens, int curr_index) {
    // validate procedure declaration: procedure (already validated), name, open brace
    // validations will be refactored into a (syntactic/semantic)evaluator in the future
    // validate size of procedure declaration
    if (curr_index + 2 > tokens.size()) {
        return -1;
    }
    // validate procedure name
    Token procedureNameToken = tokens[curr_index + 1];
    // check if name is keyword
    std::unordered_set<TokenType> keywords = {TokenType::kEntityAssign, TokenType::kEntityProcedure,
                                               TokenType::kEntityRead, TokenType::kEntityPrint,
                                               TokenType::kEntityWhile, TokenType::kEntityIf,
                                               TokenType::kEntityElse, TokenType::kEntityCall,
                                               TokenType::kEntityStmt, TokenType::kEntityConstant,
                                               TokenType::kEntityVariable};

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
    rootTNode = root;


    curr_index = curr_index + 3;
    return curr_index;
}

int AssignmentParser::parse(const std::vector<Token>& tokens, int curr_index) {
    std::shared_ptr<TNode> lhs = TNodeFactory::createNode(tokens[curr_index], lineNumber);
    std::shared_ptr<TNode> root = TNodeFactory::createNode(tokens[curr_index + 1], lineNumber);
    std::shared_ptr<TNode> parentNode = root;
    root->addChild(lhs);

    curr_index = curr_index + 2;
    std::shared_ptr<TNode> currentNode = TNodeFactory::createNode(tokens[curr_index], lineNumber);

    while (curr_index + 1 < tokens.size()) {
        Token curr = tokens[curr_index];
        Token next = tokens[curr_index + 1];
        // check next token
        if (next.tokenType == TokenType::kSepSemicolon) {
            parentNode->addChild(currentNode);
                curr_index += 1;
                break;
        } else if (next.tokenType == TokenType::kOperatorPlus || next.tokenType == TokenType::kOperatorMinus) {
            int next_index = curr_index + 1;
            // create operator node
            std::shared_ptr<TNode> subtreeRoot = TNodeFactory::createNode(next, lineNumber);
            // Add operator lhs node to operator node
            subtreeRoot->addChild(currentNode);
            // create operator rhs node
            Token subtreeRHSToken = tokens[curr_index + 2];
            if (subtreeRHSToken.tokenType != TokenType::kLiteralInteger
                && subtreeRHSToken.tokenType != TokenType::kLiteralName) {
                return -1;
            }
            std::shared_ptr<TNode> rhsSubtree = TNodeFactory::createNode(subtreeRHSToken, lineNumber);
            subtreeRoot->addChild(rhsSubtree);
            // set current node to be operator node
            currentNode = subtreeRoot;

            // loop for subsequent operators
            int temp_index = curr_index + 3;
            while (temp_index < tokens.size()
                && (tokens[temp_index].tokenType == TokenType::kOperatorPlus
                || tokens[temp_index].tokenType == TokenType::kOperatorMinus)) {
                // create operator node
                subtreeRoot = TNodeFactory::createNode(tokens[temp_index], lineNumber);
                // Add operator lhs node to operator node
                subtreeRoot->addChild(currentNode);
                // create operator rhs node
                subtreeRHSToken = tokens[temp_index + 1];
                if (subtreeRHSToken.tokenType != TokenType::kLiteralInteger
                    && subtreeRHSToken.tokenType != TokenType::kLiteralName) {
                    return -1;
                }
                rhsSubtree = TNodeFactory::createNode(subtreeRHSToken, lineNumber);
                subtreeRoot->addChild(rhsSubtree);
                // set current node to be operator node
                currentNode = subtreeRoot;
                temp_index += 2;
            }
            curr_index = temp_index - 1;
        } else {
            return -1;
        }
    }
    curr_index += 1;
    designExtractor->extractDesign(root, visitor);

    return curr_index;
}

SimpleParser::SimpleParser(WriteFacade* writeFacadePtr) : writeFacade(writeFacadePtr) { }
int SimpleParser::parse(const std::vector<Token>& tokens, int curr_index) {
    while (curr_index < tokens.size()) {
        Token curr_token = tokens[curr_index];
        if (curr_token.tokenType == TokenType::kLiteralName) {
            Token next_token = tokens.at(curr_index + 1);
            if (next_token.tokenType == TokenType::kEntityAssign) {
                assignmentParser->lineNumber = lineNumber;
                int next_index = assignmentParser->parse(tokens, curr_index);

                if (next_index == -1) {
                    throw std::runtime_error("Error: syntactic error found while building ast.");
                } else {
                    lineNumber++;
                    curr_index = next_index;
                }
            }
        } else if (curr_token.tokenType == TokenType::kEntityProcedure) {
            int next_index = procedureParser->parse(tokens, curr_index);
            if (next_index == -1) {
                throw std::runtime_error("Error: syntactic error found while building procedure ast node.");
            } else {
                curr_index = next_index;
            }
        } else {
            // currently unsupported, skip line for now
            int temp = curr_index;
            while (tokens[temp].tokenType != TokenType::kSepSemicolon
                && tokens[temp].tokenType != TokenType::kSepOpenBrace) {
                    temp++;
            }
            lineNumber++;
            curr_index = temp + 1;
//            throw std::runtime_error(
//                "Invalid token. Sorry the parser can only handle assignment statements currently.");
        }
    }
    writeFacade->storeAssignments(assignmentParser->getAssignmentStatementsHashset());
    writeFacade->storeVariables(assignmentParser->getVariablesHashset());
    writeFacade->storeConstants(assignmentParser->getConstantsHashset());
    writeFacade->storeUsesVar(assignmentParser->getAssignVarHashmap());
    writeFacade->storeUsesConst(assignmentParser->getAssignConstHashmap());
    writeFacade->storeLineUses(assignmentParser->getUsesStatementNumberHashmap());
    return curr_index;
}

std::unordered_map<std::string, std::unordered_set<std::string>> AssignmentParser::getAssignVarHashmap() {
    return visitor->getAssignVarHashmap();
}

std::unordered_map<std::string, std::unordered_set<std::string>> AssignmentParser::getAssignConstHashmap() {
    return visitor->getAssignConstHashmap();
}

std::unordered_set<std::string> AssignmentParser::getVariablesHashset() {
    return visitor->getVariablesHashset();
}

std::unordered_set<std::string> AssignmentParser::getConstantsHashset() {
    return visitor->getConstantsHashset();
}

std::unordered_map<int, std::unordered_set<std::string>> AssignmentParser::getUsesStatementNumberHashmap() {
    return visitor->getUsesStatementNumberHashmap();
}

std::unordered_map<int, std::string> AssignmentParser::getUsesStatementNumberVarHashmap() {
    return visitor->getUsesStatementNumberVarHashmap();
}

std::unordered_set<int> AssignmentParser::getAssignmentStatementsHashset() {
    return visitor->getAssignmentStatementsHashset();
}

std::unordered_map<std::string, std::unordered_set<int>> ProcedureParser::getProcedureStatementNumberHashmap() {
    return procedureVisitor->getProcedureStatementNumberHashmap();
}

void SimpleParser::tokenise(std::string code) {
    std::vector<struct Token> tokens = tokeniser.tokenise(code);

    parse(tokens, 0);
}
