#include "AssignmentParser.h"

int AssignmentParser::parse(std::vector<Token>& tokens) {
    // check if is assignment
    if (tokens.size() - index < 2) {
      return -1;
    }
    Token next_token = tokens.at(index + 1);

    if (next_token.tokenType != TokenType::kEntityAssign) {
      convertVarToEntity(&tokens[index]);
      return index;
    }

    // Validate that statement has at least 4 tokens (min: read a; lhs = rhs ;)
    if (tokens.size() - index < 4) {
        return -1;
    }

    std::shared_ptr<TNode> lhs = TNodeFactory::createNode(tokens[index], lineNumber);
    std::shared_ptr<AssignTNode> assignNode =
          std::dynamic_pointer_cast<AssignTNode>(TNodeFactory::createNode(tokens[index + 1], lineNumber));
    assignNode->addChild(lhs);
    incrementIndex();
    incrementIndex();
    ParseUtils::setValues(index, lineNumber);
    std::shared_ptr<TNode> rhs = ParseUtils::parseExpression(tokens);
    index = ParseUtils::getIndex();
    assignNode->addChild(rhs);
    // Validate that assignment ends with ;
    if (tokens[index].tokenType != TokenType::kSepSemicolon) {
        return -1;
    }
    index += 1;
    designExtractor->extractDesign(assignNode, visitor);
    followsStatementStack.top().insert(lineNumber);
    Cfg::handleStatement(lineNumber);
    lineNumber++;
    return index;
}

void AssignmentParser::convertVarToEntity(Token* token) {
  TokenType newTokenType = ParseUtils::convertLiteralToEntity(token->getValue());
  std::string newValue = token->getValue();
  Token newToken{ newTokenType, newValue };
  *token = newToken;
}
