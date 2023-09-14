#include <iostream>
#include <vector>
#include "SimpleParser.h"

bool isDebug = false;

int AssignmentParser::parse(const std::vector<Token>& tokens) {
    std::shared_ptr<TNode> lhs = TNodeFactory::createNode(tokens[curr_index]);
    std::shared_ptr<TNode> root = TNodeFactory::createNode(tokens[curr_index + 1]);
    std::shared_ptr<TNode> parentNode = root;
    root->addChild(lhs);
    std::cout << "ROOT print " << root->leftChild->content << std::endl;

    curr_index = curr_index + 2;

    while (curr_index <= tokens.size()) {
        Token curr = tokens[curr_index];
        if (isDebug) { std::cout << "curr token: " << curr.getValue() << std::endl; }

        // throw error if curr is not constant or variable
        if (curr.tokenType != TokenType::kLiteralInteger && curr.tokenType != TokenType::kLiteralName) {
            if (isDebug) { std::cout << "Invalid node. Variable or constant expected on rhs of '=' " << std::endl; }
            return -1;
        }

        Token next = tokens[curr_index + 1];
        std::shared_ptr<TNode> currentNode = TNodeFactory::createNode(tokens[curr_index]);

        // check if next is ; add curr as rhs toc root
        if (next.tokenType == TokenType::kSepSemicolon || next.lineNumber != curr.lineNumber) {
            std::cout << "ROOT print " << root->leftChild->content << std::endl;

            if (isDebug) { std::cout << "next token is end token" << std::endl; }
            std::cout << "curr node print" << std::endl;
            parentNode->addChild(currentNode);
            std::cout << "ROOT print " << root->leftChild->content << std::endl;
            root->print();
            curr_index += 1;
            std::cout << "ROOT print " << root->leftChild->content << std::endl;
            root->print();
            break;
        }

        // throw error if next is not operator
        if (next.tokenType != TokenType::kOperatorPlus && next.tokenType != TokenType::kOperatorMinus) {
            if (isDebug) { std::cout << "Invalid node. Operator expected." << std::endl; }
            return -1;
        }

        if (isDebug) { std::cout << "next token is operator" << std::endl; }

        // add operator as rhs of root and add curr as lhs of rhs
        std::shared_ptr<TNode> rhs = TNodeFactory::createNode(next);

        std::cout << "rhs node print" << std::endl;
        rhs->print();
        std::cout << "curr node print" << std::endl;
        currentNode->print();

        rhs->addChild(currentNode);
        parentNode->addChild(rhs);

        // update lhs and root
        parentNode = rhs;

        // update curr_index
        curr_index += 2;
    }
    curr_index += 1;
    if (isDebug) { std::cout << "next index" << curr_index << std::endl; }
    if (isDebug) { std::cout << "root " << root->content << std::endl; }
    std::cout << "ROOT print " << root->leftChild->content << std::endl;
    root->print();
    designExtractor->extractDesign(root, visitor);

    return curr_index;
}

SimpleParser::SimpleParser(WriteFacade* writeFacadePtr) : writeFacade(writeFacadePtr) { }
int SimpleParser::parse(const std::vector<Token>& tokens) {
    while (curr_index < tokens.size()) {
        Token curr_token = tokens[curr_index];

        if (isDebug) { std::cout << "curr token: " << curr_token.getValue() << std::endl; }
        if (curr_token.tokenType == TokenType::kLiteralName) {
            if (isDebug) { std::cout << "is variable " << std::endl; }
            if (isDebug) { std::cout << "curr index: " << curr_index << std::endl; }

            Token next_token = tokens.at(curr_index + 1);
            if (isDebug) { std::cout << "next token: " << curr_token.getValue() << std::endl; }
            if (next_token.tokenType == TokenType::kEntityAssign) {
                if (isDebug) { std::cout << "next token is equal " << std::endl; }

                int next_index = assignmentParser->parse(tokens);

                if (next_index == -1) {
                    if (isDebug) { std::cout << "Error: Invalid token in RHS of assignment" << std::endl; }
                    throw std::runtime_error("Error: syntactic error found while building ast.");
                } else {
                    curr_index = next_index;
                }
            }
        } else {
            throw InvalidStatementError(
                "Invalid token. Sorry the parser can only handle assignment statements currently.");
        }
    }
    std::cout << "test" << std::endl;
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
    std::cout << "getVariablesHashset called" << std::endl;
    return visitor->getVariablesHashset();
}

std::unordered_set<std::string> AssignmentParser::getConstantsHashset() {
    return visitor->getConstantsHashset();
}

void SimpleParser::tokenise(std::string code) {
    std::vector<struct Token> tokens = tokeniser.tokenise(code);
    try { parse(tokens); }
    catch (InvalidStatementError ise) {
        ise.log();
    }
}
