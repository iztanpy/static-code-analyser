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

    while (curr_index <= tokens.size()) {
        Token curr = tokens[curr_index];
        // throw error if curr is not constant or variable
        if (curr.tokenType != TokenType::kLiteralInteger && curr.tokenType != TokenType::kLiteralName) {
            return -1;
        }

        Token next = tokens[curr_index + 1];
        std::shared_ptr<TNode> currentNode = TNodeFactory::createNode(tokens[curr_index]);

        // check if next is ; add curr as rhs toc root
        if (next.tokenType == TokenType::kSepSemicolon || next.lineNumber != curr.lineNumber) {

            parentNode->addChild(currentNode);
            curr_index += 1;
            break;
        }

        // throw error if next is not operator
        if (next.tokenType != TokenType::kOperatorPlus && next.tokenType != TokenType::kOperatorMinus) {
            return -1;
        }
        // add operator as rhs of root and add curr as lhs of rhs
        std::shared_ptr<TNode> rhs = TNodeFactory::createNode(next);

        currentNode->print();

        rhs->addChild(currentNode);
        parentNode->addChild(rhs);

        // update lhs and root
        parentNode = rhs;

        // update curr_index
        curr_index += 2;
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
            throw std::runtime_error(
                "Invalid token. Sorry the parser can only handle assignment statements currently.");
            lineNumber++;
        }
    }
    writeFacade->storeVariables(assignmentParser->getVariablesHashset());
    writeFacade->storeConstants(assignmentParser->getConstantsHashset());
    writeFacade->storeUsesVar(assignmentParser->getAssignVarHashmap());
    writeFacade->storeUsesConst(assignmentParser->getAssignConstHashmap());
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

std::unordered_map<std::string, std::unordered_set<int>> ProcedureParser::getProcedureStatementNumberHashmap() {
    return procedureVisitor->getProcedureStatementNumberHashmap();
};

void SimpleParser::tokenise(std::string code) {
    std::vector<struct Token> tokens = tokeniser.tokenise(code);
    parse(tokens, 0);
}
