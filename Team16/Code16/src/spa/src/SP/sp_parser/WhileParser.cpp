// ai-gen start (gpt3, 2)
#include "WhileParser.h"

int WhileParser::parse(std::vector<Token>& tokens) {
    // Validate that statement has at least 6 tokens (min: while ( a ) { })
    if (tokens.size() - index < 5) {
      return -1;
    }

    // Create a new while node
    std::shared_ptr<TNode> whileNode = TNodeFactory::createNode(tokens[index], lineNumber);
    index++;

    // Validate open parenthesis
    if (tokens[index].tokenType != TokenType::kSepOpenParen) {
        throw InvalidSyntaxError();
    }

    // Add to stacks
    parentStatementStack.push(lineNumber);
    controlStructureStack.push("while");

    // Set up ParseUtils
    ParseUtils::setValues(index, lineNumber);
    ParseUtils::setUpCondIndexMap(tokens);
    index++;
    ParseUtils::setValues(index, lineNumber);

    std::shared_ptr<TNode> whileCondNode = ParseUtils::parseCondExpression(tokens);
    index = ParseUtils::getIndex();
    whileNode->addChild(whileCondNode);

    // Validate close parenthesis
    if (tokens[index].tokenType != TokenType::kSepCloseParen) {
        throw InvalidSyntaxError();
    }
    index++;

    // Validate open braces
    if (tokens[index].tokenType != TokenType::kSepOpenBrace) {
        throw InvalidSyntaxError();
    }
    index++;

    designExtractor->extractDesign(whileNode, visitor);
    followsStatementStack.top().insert(lineNumber);
    std::set<int> whileFollowsSet;
    followsStatementStack.push(whileFollowsSet);
    Cfg::handleWhileStatement(lineNumber);
    currWhileDepth++;
    lineNumber++;
    return index;
}
