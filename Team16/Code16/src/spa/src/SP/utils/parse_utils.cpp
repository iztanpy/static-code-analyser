#include "SP/utils/parse_utils.h"

std::unordered_map<int, std::vector<int>> parse_utils::condIndexMap = std::unordered_map<int, std::vector<int>>();

bool parse_utils::isVarOrConst(const Token& token) {
  return token.tokenType == TokenType::kLiteralName ||
      token.tokenType == TokenType::kLiteralInteger;
}

bool parse_utils::isPlusOrMinus(const Token& token) {
  return token.tokenType == TokenType::kOperatorPlus ||
      token.tokenType == TokenType::kOperatorMinus;
}

bool parse_utils::isMultiplyDivideOrModulo(const Token& token) {
  return token.tokenType == TokenType::kOperatorMultiply ||
      token.tokenType == TokenType::kOperatorDivide ||
      token.tokenType == TokenType::kOperatorMod;
}

bool parse_utils::isCondExpressionOperator(const Token& token) {
  return token.tokenType == TokenType::kOperatorLogicalAnd ||
      token.tokenType == TokenType::kOperatorLogicalOr;
}

bool parse_utils::isRelFactorOperator(const Token& token) {
  return token.tokenType == TokenType::kOperatorNotEqual ||
      token.tokenType == TokenType::kOperatorGreater ||
      token.tokenType == TokenType::kOperatorLess ||
      token.tokenType == TokenType::kOperatorLessEqual ||
      token.tokenType == TokenType::kOperatorGreaterEqual ||
      token.tokenType == TokenType::kOperatorEqual;
}

// Initialize variables
int parse_utils::index = 0;
int parse_utils::lineNumber = 0;
static std::string* procedureName;

void parse_utils::incrementIndex() {
  index++;
}

int parse_utils::getIndex() {
  return index;
}

std::string parse_utils::getProcedureName() {
  return procedureName;
}

void parse_utils::setValues(int index, int lineNumber) {
  parse_utils::index = index;
  parse_utils::lineNumber = lineNumber;
}

void parse_utils::setProcedureName(std::string procedureName) {
  parse_utils::procedureName = procedureName;
}

std::shared_ptr<TNode> parse_utils::parseExpression(const std::vector<Token>& tokens) {
  std::shared_ptr<TNode> tree = parseTerm(tokens);
  while (parse_utils::isPlusOrMinus(tokens[index])) {
    std::shared_ptr<TNode> operatorNode = TNodeFactory::createNode(tokens[index], lineNumber);
    operatorNode->addChild(tree);
    incrementIndex();
    std::shared_ptr<TNode> rhs = parseTerm(tokens);
    operatorNode->addChild(rhs);
    tree = operatorNode;
  }

  return tree;
}

std::shared_ptr<TNode> parse_utils::parseTerm(const std::vector<Token>& tokens) {
  std::shared_ptr<TNode> tree = parseFactor(tokens);
  while (parse_utils::isMultiplyDivideOrModulo(tokens[index])) {
    std::shared_ptr<TNode> operatorNode = TNodeFactory::createNode(tokens[index], lineNumber);
    operatorNode->addChild(tree);
    incrementIndex();
    std::shared_ptr<TNode> rhs = parseFactor(tokens);
    operatorNode->addChild(rhs);
    tree = operatorNode;
  }

  return tree;
}

std::shared_ptr<TNode> parse_utils::parseFactor(const std::vector<Token>& tokens) {
  std::shared_ptr<TNode> node = nullptr;
  if (tokens[index].tokenType == TokenType::kSepOpenParen) {
//    node = TNodeFactory::createNode(tokens[index], lineNumber);
    incrementIndex();
//    std::shared_ptr<TNode> childNode = nullptr;
    node = parseExpression(tokens);
//    node->addChild(childNode);
    if (tokens[index].tokenType != TokenType::kSepCloseParen) {
      throw InvalidSyntaxError();
    }
    incrementIndex();
    return node;
  } else {
    if (parse_utils::isVarOrConst(tokens[index])) {
      node = TNodeFactory::createNode(tokens[index], lineNumber);
      incrementIndex();
    }
  }

  if (node == nullptr) {
    throw InvalidSyntaxError();
  }

  return node;
}

std::shared_ptr<TNode> parse_utils::parseCondExpression(const std::vector<Token>& tokens) {
  std::shared_ptr<TNode> tree = nullptr;
  int i = index;
  bool we = condIndexMap.find(index) != condIndexMap.end();

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
  } else if (condIndexMap.find(index) != condIndexMap.end()) {
    int& condEndIndex = condIndexMap[index][2];
    int& condOprIndex = condIndexMap[index][1];

    // this is a conditional expression
    incrementIndex();
    // lhs conditional expression
    tree = parseCondExpression(tokens);
//      if (tokens[index].tokenType != TokenType::kSepCloseParen) {
//          throw InvalidSyntaxError();
//      }

    // conditional operator
    incrementIndex();
    if (index != condOprIndex) throw InvalidSyntaxError();
    std::shared_ptr<TNode> operatorNode = TNodeFactory::createNode(tokens[index], lineNumber);
    operatorNode->addChild(tree);
    incrementIndex();

    // rhs conditional expression
    incrementIndex();
    std::shared_ptr<TNode> rhs = parseCondExpression(tokens);
    operatorNode->addChild(rhs);
    tree = operatorNode;

    // check end of conditional expression
    int i = index;
    if (index != condEndIndex) throw InvalidSyntaxError();
    incrementIndex();
//
//      // conditional operator
//      if (parse_utils::isCondExpressionOperator(tokens[index])) {
//          std::shared_ptr<TNode> operatorNode = TNodeFactory::createNode(tokens[index], lineNumber);
//          operatorNode->addChild(tree);
//          incrementIndex();
//
//          if (tokens[index].tokenType == TokenType::kSepOpenParen) {
//              incrementIndex();
//              // rhs conditional expression
//              std::shared_ptr<TNode> rhs = parseCondExpression(tokens);
//              operatorNode->addChild(rhs);
//              tree = operatorNode;
//              if (tokens[index].tokenType != TokenType::kSepCloseParen) {
//                  throw InvalidSyntaxError();
//              }
//              incrementIndex();
//          } else {
//              throw InvalidSyntaxError();
//          }
//      }
  } else {
    tree = parseRelExpression(tokens);
  }
  return tree;
}

std::shared_ptr<TNode> parse_utils::parseRelExpression(const std::vector<Token>& tokens) {
  std::shared_ptr<TNode> tree = parseRelFactor(tokens);

  if (parse_utils::isRelFactorOperator(tokens[index])) {
    std::shared_ptr<TNode> operatorNode = TNodeFactory::createNode(tokens[index], lineNumber);
    operatorNode->addChild(tree);
    incrementIndex();
    std::shared_ptr<TNode> rhs = parseRelFactor(tokens);
    operatorNode->addChild(rhs);
    tree = operatorNode;
  }

  return tree;
}

std::shared_ptr<TNode> parse_utils::parseRelFactor(const std::vector<Token>& tokens) {
  std::shared_ptr<TNode> node = nullptr;
  node = parseExpression(tokens);

  if (node == nullptr) {
    throw InvalidSyntaxError();
  }

  return node;
}

std::unordered_map<std::string, TokenType> parse_utils::entityMap = std::unordered_map<std::string, TokenType>();

void parse_utils::setUpEntityMap() {
  entityMap["if"] = TokenType::kEntityIf;
  entityMap["then"] = TokenType::kEntityThen;
  entityMap["while"] = TokenType::kEntityWhile;
  entityMap["else"] = TokenType::kEntityElse;
  entityMap["read"] = TokenType::kEntityRead;
  entityMap["procedure"] = TokenType::kEntityProcedure;
  entityMap["print"] = TokenType::kEntityPrint;
  entityMap["call"] = TokenType::kEntityCall;
}

TokenType parse_utils::convertLiteralToEntity(const std::string& value) {
  if (entityMap.empty()) {
    setUpEntityMap();
  }
  if (entityMap.find(value) != entityMap.end()) {
    TokenType type = entityMap[value];
    return type;
  } else {
    throw InvalidSyntaxError();
  }
}

void parse_utils::setUpCondIndexMap(const std::vector<Token>& tokens) {
  condIndexMap.clear();
  int i = index;
  std::shared_ptr<std::stack<int>> openParenIndexStack = std::make_shared<std::stack<int>>();
  std::shared_ptr<std::stack<std::vector<int>>> currCondIndex = std::make_shared<std::stack<std::vector<int>>>();
  while (i < tokens.size()) {
    if (tokens[i].tokenType == TokenType::kSepOpenParen) {
      openParenIndexStack->push(i);
    } else if (tokens[i].tokenType == TokenType::kSepCloseParen) {
      int& openParenIndex = openParenIndexStack->top();
      openParenIndexStack->pop();
      if (openParenIndexStack->empty()) {
        break;
      } else if (!currCondIndex.get()->empty() && openParenIndex == currCondIndex->top()[1] + 1) {
        // closing a cond expression
        std::vector<int>& curr = currCondIndex->top();
        curr.push_back(i);
        condIndexMap[curr[0]] = curr;
        currCondIndex->pop();
      } else if (i + 2 < tokens.size()
          && isCondExpressionOperator(tokens[i + 1])
          && tokens[i + 2].tokenType == TokenType::kSepOpenParen) {
        // cond expression detected save [cond start index, cond operator index]
        std::vector<int> curr = std::vector<int>();
        curr.push_back(openParenIndex);
        curr.push_back(i + 1);
        currCondIndex->push(curr);
        i++;
      }
    } else if (isCondExpressionOperator(tokens[i])) {
      throw InvalidSyntaxError();
    }
    i++;
  }
}
