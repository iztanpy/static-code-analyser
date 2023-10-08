#include "ElseParser.h"

int ElseParser::parse(std::vector<Token>& tokens) {
  if (!controlStructureStack.empty() && controlStructureStack.top() != "if") {
    throw InvalidSyntaxError();
  }
  if (Parser::index + 1 < tokens.size()
      && tokens[Parser::index + 1].tokenType != TokenType::kSepOpenBrace) {
    throw InvalidSyntaxError();
  }
  if (Parser::index - 1 > 0
      && tokens[Parser::index - 1].tokenType != TokenType::kSepCloseBrace) {
    throw InvalidSyntaxError();
  }
  std::set<int> elseFollowsSet;
  followsStatementStack.push(elseFollowsSet);
  Parser::index += 2;  // skip over the next open brace
  return index;
}
