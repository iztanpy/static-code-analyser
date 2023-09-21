#include "AssignmentParser.h"

void AssignmentParser::incrementIndex() {
  index++;
}

std::shared_ptr<TNode> AssignmentParser::parseExpression(const std::vector<Token>& tokens) {
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

std::shared_ptr<TNode> AssignmentParser::parseTerm(const std::vector<Token>& tokens) {
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

std::shared_ptr<TNode> AssignmentParser::parseFactor(const std::vector<Token>& tokens) {
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

int AssignmentParser::parse(const std::vector<Token>& tokens, int curr_index) {
    // Validate that statement has at least 4 tokens (min: lhs = rhs ;)
    if (tokens.size() - index < 3) {
        return -1;
    }
    std::shared_ptr<TNode> lhs = TNodeFactory::createNode(tokens[index], lineNumber);
    std::shared_ptr<TNode> assignNode = TNodeFactory::createNode(tokens[index + 1], lineNumber);
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

    return index;
}

