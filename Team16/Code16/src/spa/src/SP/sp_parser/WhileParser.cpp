// ai-gen start (gpt3, 2)
#include "WhileParser.h"

int WhileParser::parse(const std::vector<Token>& tokens, int curr_index) {
    // Validate that statement has at least 6 tokens (min: while ( a ) { })
    if (tokens.size() - index < 5) {
      return -1;
    }
    index++;

    // Validate open parenthesis
    if (tokens[index].tokenType != TokenType::kSepOpenParen) {
        throw InvalidSyntaxError();
    }
    index++;

    ParseUtils::setValues(index, lineNumber);
    std::shared_ptr<TNode> whileCondNode = ParseUtils::parseCondExpression(tokens);
    index = ParseUtils::getIndex();

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

    designExtractor->extractDesign(whileCondNode, visitor);

    return index;
}
