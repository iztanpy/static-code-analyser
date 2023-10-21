#include "IfParser.h"

int IfParser::parse(std::vector<Token>& tokens) {
  // Validate that statement has at least 9 tokens (min: If ( a ) { } else { } )
  if (tokens.size() - index < 8) {
    return -1;
  }

  // Create a new if node
  std::shared_ptr<TNode> ifNode = TNodeFactory::createNode(tokens[index], lineNumber);
  index++;

  // Validate open parenthesis
  if (tokens[index].tokenType != TokenType::kSepOpenParen) {
    throw InvalidSyntaxError();
  }

  // Add to stacks
  parentStatementStack.push(lineNumber);
  Parser::controlStructureStack.push("if");

  // Set up ParseUtils
  ParseUtils::setValues(index, lineNumber);
  ParseUtils::setUpCondIndexMap(tokens);
  index++;
  ParseUtils::setValues(index, lineNumber);

  std::shared_ptr<TNode> ifCondNode = ParseUtils::parseCondExpression(tokens);
  index = ParseUtils::getIndex();
  ifNode->addChild(ifCondNode);

  // Validate close parenthesis
  if (tokens[index].tokenType != TokenType::kSepCloseParen) {
      throw InvalidSyntaxError();
  }
  index++;

  // Validate 'then' keyword
  if (tokens[index].getValue() != "then") {
      throw InvalidSyntaxError();
  }
  index++;

  // Validate open braces
  if (tokens[index].tokenType != TokenType::kSepOpenBrace) {
    throw InvalidSyntaxError();
  }
  index++;

  designExtractor->extractDesign(ifNode, visitor);
  followsStatementStack.top().insert(lineNumber);
  std::set<int> ifFollowsSet;
  followsStatementStack.push(ifFollowsSet);
  Cfg::handleIfStatement(lineNumber);

  currIfDepth++;
  lineNumber++;
  return index;
}
