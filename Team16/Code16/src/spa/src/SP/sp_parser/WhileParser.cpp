// ai-gen start (gpt3, 2)
#include "WhileParser.h"

int WhileParser::parse(std::vector<Token>& tokens, int curr_index) {
    // Validate that statement has at least 6 tokens (min: while ( a ) { })
    if (tokens.size() - index < 5) {
      return -1;
    }

    // Create a new while node
    std::shared_ptr<TNode> whileNode = TNodeFactory::createNode(tokens[index], lineNumber);
    index++;

    // Validate open parenthesis
    if (tokens[index].tokenType != TokenType::kSepOpenParen) {
      throw std::runtime_error("Syntactic error! We don't support anything and everything.");
    }
    index++;

    ParseUtils::setValues(index, lineNumber);
    std::shared_ptr<TNode> whileCondNode = ParseUtils::parseCondExpression(tokens);
    index = ParseUtils::getIndex();
    whileNode->addChild(whileCondNode);

    // Validate close parenthesis
    if (tokens[index].tokenType != TokenType::kSepCloseParen) {
      throw std::runtime_error("Syntactic error! We don't support anything and everything.");
    }
    index++;

    // Validate open braces
    if (tokens[index].tokenType != TokenType::kSepOpenBrace) {
      throw std::runtime_error("Syntactic error! We don't support anything and everything.");
    }
    index++;

    designExtractor->extractDesign(whileNode, visitor);

    return index;
}
