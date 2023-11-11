#include "SP/sp_tokeniser/token.h"
#include "SP/tnode.h"

#include "catch.hpp"

TEST_CASE("Test Token to TNode conversion") {
  TokenType variableType = TokenType::kLiteralName;
  TokenType constantType = TokenType::kLiteralInteger;
  TokenType endType = TokenType::kSepSemicolon;
  TokenType plusType = TokenType::kOperatorPlus;
  TokenType equalType = TokenType::kEntityAssign;

  Token tokenX = Token(variableType, "x");
  Token tokenEqual = Token(equalType);
  Token tokenPlus = Token(plusType);
  Token token1 = Token(constantType, "1");
  Token tokenEnd = Token(endType);

  auto* nodeX = new VariableTNode(1, tokenX.value);
  auto* nodeEqual = new AssignTNode(1);
  auto* nodePlus = new PlusTNode(1);
  auto* node1 = new ConstantTNode(1, "1");
  std::shared_ptr<TNode> nodeXCreated = TNodeFactory::createNode(tokenX, 1);
  std::shared_ptr<TNode> nodeEqualCreated = TNodeFactory::createNode(tokenEqual, 1);
  std::shared_ptr<TNode> nodePlusCreated = TNodeFactory::createNode(tokenPlus, 1);
  std::shared_ptr<TNode> node1Created = TNodeFactory::createNode(token1, 1);

  REQUIRE(nodeXCreated->type == TokenType::kLiteralName);
  REQUIRE(nodeXCreated->getContent() == tokenX.getValue());
  REQUIRE(nodeEqualCreated->type == TokenType::kEntityAssign);
  REQUIRE(nodeEqualCreated->getContent().empty());
  REQUIRE(nodePlusCreated->type == TokenType::kOperatorPlus);
  REQUIRE(node1Created->type == TokenType::kLiteralInteger);
}


