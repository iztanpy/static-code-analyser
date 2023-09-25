#include "ParseUtils.h"

bool ParseUtils::isVarOrConst(const Token& token) {
  return token.tokenType == TokenType::kLiteralName ||
      token.tokenType == TokenType::kLiteralInteger;
}

bool ParseUtils::isPlusOrMinus(const Token& token) {
  return token.tokenType == TokenType::kOperatorPlus ||
      token.tokenType == TokenType::kOperatorMinus;
}

bool ParseUtils::isMultiplyDivideOrModulo(const Token& token) {
  return token.tokenType == TokenType::kOperatorMultiply ||
      token.tokenType == TokenType::kOperatorDivide ||
      token.tokenType == TokenType::kOperatorMod;
}

bool ParseUtils::isCondExpressionOperator(const Token& token) {
  return token.tokenType == TokenType::kOperatorLogicalAnd ||
      token.tokenType == TokenType::kOperatorLogicalOr;
}

bool ParseUtils::isRelFactorOperator(const Token& token) {
  return token.tokenType == TokenType::kOperatorNotEqual ||
      token.tokenType == TokenType::kOperatorGreater ||
      token.tokenType == TokenType::kOperatorLess ||
      token.tokenType == TokenType::kOperatorLessEqual ||
      token.tokenType == TokenType::kOperatorGreaterEqual ||
      token.tokenType == TokenType::kOperatorEqual;
}

int ParseUtils::index = 0;
int ParseUtils::lineNumber = 0;

void ParseUtils::incrementIndex() {
  index++;
}

int ParseUtils::getIndex() {
  return index;
}

void ParseUtils::setValues(int index, int lineNumber) {
  ParseUtils::index = index;
  ParseUtils::lineNumber = lineNumber;
}

std::shared_ptr<TNode> ParseUtils::parseExpression(const std::vector<Token>& tokens) {
  std::shared_ptr<TNode> tree = parseTerm(tokens);
  while (ParseUtils::isPlusOrMinus(tokens[index])) {
    std::shared_ptr<TNode> operatorNode = TNodeFactory::createNode(tokens[index], lineNumber);
    operatorNode->addChild(tree);
    incrementIndex();
    std::shared_ptr<TNode> rhs = parseTerm(tokens);
    operatorNode->addChild(rhs);
    tree = operatorNode;
  }

  return tree;
}

std::shared_ptr<TNode> ParseUtils::parseTerm(const std::vector<Token>& tokens) {
  std::shared_ptr<TNode> tree = parseFactor(tokens);
  while (ParseUtils::isMultiplyDivideOrModulo(tokens[index])) {
    std::shared_ptr<TNode> operatorNode = TNodeFactory::createNode(tokens[index], lineNumber);
    operatorNode->addChild(tree);
    incrementIndex();
    std::shared_ptr<TNode> rhs = parseFactor(tokens);
    operatorNode->addChild(rhs);
    tree = operatorNode;
  }

  return tree;
}

std::shared_ptr<TNode> ParseUtils::parseFactor(const std::vector<Token>& tokens) {
  std::shared_ptr<TNode> node = nullptr;
  if (tokens[index].tokenType == TokenType::kSepOpenParen) {
    incrementIndex();
    node = parseExpression(tokens);
    if (tokens[index].tokenType != TokenType::kSepCloseParen) {
      throw InvalidSyntaxError();
    }
    incrementIndex();
    return node;
  } else {
    if (ParseUtils::isVarOrConst(tokens[index])) {
      node = TNodeFactory::createNode(tokens[index], lineNumber);
      incrementIndex();
    }
  }

  if (node == nullptr) {
    throw InvalidSyntaxError();
  }

  return node;
}

std::shared_ptr<TNode> ParseUtils::parseCondExpression(const std::vector<Token>& tokens) {
  std::shared_ptr<TNode> tree = nullptr;

  // check for !
  if (tokens[index].tokenType == TokenType::kOperatorLogicalNot) {
    std::shared_ptr<TNode> operatorNode = TNodeFactory::createNode(tokens[index], lineNumber);
    incrementIndex();

    if (tokens[index].tokenType == TokenType::kSepOpenParen) {
      incrementIndex();
      std::shared_ptr<TNode> child = parseCondExpression(tokens);
      operatorNode->addChild(child);
      tree = operatorNode;
      if (tokens[index].tokenType != TokenType::kSepCloseParen) {
        throw InvalidSyntaxError();
      }
      incrementIndex();
    } else {
      throw InvalidSyntaxError();
    }
  } else if (tokens[index].tokenType == TokenType::kSepOpenParen) {
    incrementIndex();
    // lhs conditional expression
    tree = parseCondExpression(tokens);
    if (tokens[index].tokenType != TokenType::kSepCloseParen) {
      throw InvalidSyntaxError();
    }
    incrementIndex();

    // conditional operator
    while (ParseUtils::isCondExpressionOperator(tokens[index])) {
      std::shared_ptr<TNode> operatorNode = TNodeFactory::createNode(tokens[index], lineNumber);
      operatorNode->addChild(tree);
      incrementIndex();

      if (tokens[index].tokenType == TokenType::kSepOpenParen) {
        incrementIndex();
        // rhs conditional expression
        std::shared_ptr<TNode> rhs = parseCondExpression(tokens);
        operatorNode->addChild(rhs);
        tree = operatorNode;
        if (tokens[index].tokenType != TokenType::kSepCloseParen) {
          throw InvalidSyntaxError();
        }
        incrementIndex();
      } else {
        throw InvalidSyntaxError();
      }
    }
  } else {
    tree = parseRelExpression(tokens);
  }
  return tree;
}

std::shared_ptr<TNode> ParseUtils::parseRelExpression(const std::vector<Token>& tokens) {
  std::shared_ptr<TNode> tree = parseRelFactor(tokens);

  while (ParseUtils::isRelFactorOperator(tokens[index])) {
    std::shared_ptr<TNode> operatorNode = TNodeFactory::createNode(tokens[index], lineNumber);
    operatorNode->addChild(tree);
    incrementIndex();
    std::shared_ptr<TNode> rhs = parseRelFactor(tokens);
    operatorNode->addChild(rhs);
    tree = operatorNode;
  }

  return tree;
}

std::shared_ptr<TNode> ParseUtils::parseRelFactor(const std::vector<Token>& tokens) {
  std::shared_ptr<TNode> node = nullptr;
  node = parseExpression(tokens);

  if (node == nullptr) {
    throw InvalidSyntaxError();
  }

  return node;
}


std::vector<TokenType> unconfirmed_entities = {
    TokenType::kEntityIf,
    TokenType::kEntityElse,
    TokenType::kEntityThen,
    TokenType::kEntityWhile,
    TokenType::kEntityRead,
    TokenType::kEntityProcedure,
    TokenType::kEntityPrint,
    TokenType::kEntityCall,
};

TokenType ParseUtils::convertLiteralToEntity(std::string value) {
    if (value == "if") {
      return TokenType::kEntityIf;
    } else if (value == "then") {
        return TokenType::kEntityThen;
    } else if (value == "while") {
        return TokenType::kEntityWhile;
    } else if (value == "else") {
        return TokenType::kEntityElse;
    } else if (value == "read") {
        return TokenType::kEntityRead;
    } else if (value == "procedure") {
        return TokenType::kEntityProcedure;
    } else if (value == "print") {
        return TokenType::kEntityPrint;
    } else if (value == "call") {
        return TokenType::kEntityCall;
    } else {
        throw InvalidSyntaxError();
    }
}

