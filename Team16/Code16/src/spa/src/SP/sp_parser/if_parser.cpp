#include "SP/sp_parser/if_parser.h"

int if_parser::parse(std::vector<Token>& tokens) {
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

  // Set up parse_utils
  parse_utils::setValues(index, lineNumber);
  parse_utils::setUpCondIndexMap(tokens);
  index++;
  parse_utils::setValues(index, lineNumber);

  std::shared_ptr<TNode> ifCondNode = parse_utils::parseCondExpression(tokens);
  index = parse_utils::getIndex();
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

  design_extractor::extractDesign(ifNode, visitor);
  followsStatementStack.top().insert(lineNumber);
  std::set<int> ifFollowsSet;
  followsStatementStack.push(ifFollowsSet);
  cfg::handleIfStatement(lineNumber);

  currIfDepth++;
  lineNumber++;
  return index;
}
