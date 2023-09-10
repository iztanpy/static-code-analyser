#include "SP/Token.h"
#include "SP/TNode.h"

#include "catch.hpp"

TEST_CASE("Test Token to TNode conversion") {
    TokenType variableType = TokenType::kLiteralName;
    TokenType constantType = TokenType::kLiteralInteger;
    TokenType endType = TokenType::kSepSemicolon;
    TokenType plusType = TokenType::kOperatorPlus;
    TokenType equalType = TokenType::kEntityAssign;

    Token tokenX = Token(variableType, "x", 0);
    Token tokenEqual = Token(equalType, 0);
    Token tokenPlus = Token(plusType);
    Token token1 = Token(constantType, "1", 0);
    Token tokenEnd = Token(endType);

    auto* nodeX = new VariableTNode(tokenX.value);
    auto* nodeEqual = new AssignTNode();
    auto* nodePlus = new PlusTNode();
    auto* node1 = new ConstantTNode("1");
    std::shared_ptr<TNode> nodeXCreated = TNodeFactory::createNode(tokenX);
    std::shared_ptr<TNode> nodeEqualCreated = TNodeFactory::createNode(tokenEqual);
    std::shared_ptr<TNode> nodePlusCreated = TNodeFactory::createNode(tokenPlus);
    std::shared_ptr<TNode> node1Created = TNodeFactory::createNode(token1);

    REQUIRE(nodeXCreated->type == TokenType::kLiteralName);
    REQUIRE(nodeXCreated->getContent() == tokenX.getValue());
//    REQUIRE(nodeX2 == nodeX);

    REQUIRE(nodeEqualCreated->type == TokenType::kEntityAssign);
    REQUIRE(nodeEqualCreated->getContent().empty());

    REQUIRE(nodePlusCreated->type == TokenType::kOperatorPlus);
    REQUIRE(nodePlusCreated->getContent().empty());

    REQUIRE(node1Created->type == TokenType::kLiteralInteger);
    REQUIRE(node1Created->getContent() == token1.getValue());
}


