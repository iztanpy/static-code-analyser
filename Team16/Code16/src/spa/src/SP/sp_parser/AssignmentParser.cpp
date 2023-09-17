#include "AssignmentParser.h"

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
