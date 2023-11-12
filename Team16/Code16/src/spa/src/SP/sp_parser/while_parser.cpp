// ai-gen start (gpt3, 2)
#include "SP/sp_parser/while_parser.h"

int while_parser::parse(std::vector<Token>& tokens) {
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

  // Set up parse_utils
  parse_utils::setValues(index, lineNumber);
  parse_utils::setUpCondIndexMap(tokens);
  index++;
  parse_utils::setValues(index, lineNumber);

  std::shared_ptr<TNode> whileCondNode = parse_utils::parseCondExpression(tokens);
  index = parse_utils::getIndex();
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

  design_extractor::extractDesign(whileNode, visitor);
  followsStatementStack.top().insert(lineNumber);
  std::set<int> whileFollowsSet;
  followsStatementStack.push(whileFollowsSet);
  cfg::handleWhileStatement(lineNumber);
  currWhileDepth++;
  lineNumber++;
  return index;
}
