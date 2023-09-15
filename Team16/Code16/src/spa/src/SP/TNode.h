#ifndef TEAM16_CODE16_SRC_SPA_SRC_SP_TNODE_H_
#define TEAM16_CODE16_SRC_SPA_SRC_SP_TNODE_H_

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Token.h"

class ASTVisitor;
enum class TokenType;

class TNode {
 public:
  explicit TNode(int statementNumber);
  virtual ~TNode() = default;
  int statementNumber = 0;
  TokenType type = TokenType::kUnknownTokenType;
  std::string content = "";
  std::shared_ptr<TNode> leftChild;
  std::shared_ptr<TNode> rightChild;
  virtual void accept(ASTVisitor* visitor, std::string& key) const = 0;
  void addChild(const std::shared_ptr<TNode>& child) {
      if (!leftChild) {
          leftChild = child;
      } else if (!rightChild) {
          rightChild = child;
      } else {
          throw std::invalid_argument("Error: TNode already has two children");
      }
  }

  bool operator==(const TNode& other) const {
      return type == other.type && content == other.content;
  }
  virtual std::string getContent() const {
      return content;
  }
};

class ProcedureTNode : public TNode {
 public:
    explicit ProcedureTNode(const std::string& procedureName) : TNode(0) {
        type = TokenType::kEntityProcedure;
        content = procedureName;
    }
    void accept(ASTVisitor* visitor, std::string& key) const override;
};

class AssignTNode : public TNode {
 public:
  explicit AssignTNode(int statementNumber) : TNode(statementNumber) {
      type = TokenType::kEntityAssign;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
};

class VariableTNode : public TNode {
 public:
  explicit VariableTNode(int statementNumber, const std::string& c) : TNode(statementNumber) {
      type = TokenType::kLiteralName;
      content = c;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
};

class ConstantTNode : public TNode {
 public:
  explicit ConstantTNode(int statementNumber, const std::string& c) : TNode(statementNumber) {
      type = TokenType::kLiteralInteger;
      content = c;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
};

class PlusTNode : public TNode {
 public:
  explicit PlusTNode(int statementNumber) : TNode(statementNumber) {
      type = TokenType::kOperatorPlus;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
  std::string getContent() const override {
      return leftChild->getContent() + " + " + rightChild->getContent();
  }
};

class MinusTNode : public TNode {
 public:
  explicit MinusTNode(int statementNumber) : TNode(statementNumber) {
      type = TokenType::kOperatorMinus;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
  std::string getContent() const override {
      return leftChild->getContent() + " - " + rightChild->getContent();
  }
};

class TNodeFactory {
 public:
  static std::shared_ptr<TNode> createNode(const Token& token, int statementNumber) {
      switch (token.tokenType) {
          case TokenType::kEntityProcedure:
              return std::make_shared<ProcedureTNode>(token.value);
          case TokenType::kEntityAssign:
              return std::make_shared<AssignTNode>(statementNumber);
          case TokenType::kLiteralName: {
              std::string c = token.value;
              return std::make_shared<VariableTNode>(statementNumber, c);
          }
          case TokenType::kLiteralInteger: {
              std::string c = token.value;
              return std::make_shared<ConstantTNode>(statementNumber, c);
          }
          case TokenType::kOperatorPlus:
              return std::make_shared<PlusTNode>(statementNumber);
          case TokenType::kOperatorMinus:
              return std::make_shared<MinusTNode>(statementNumber);
          default:
              throw std::invalid_argument("Error: unknown token type");
      }
  }
};

#endif  // TEAM16_CODE16_SRC_SPA_SRC_SP_TNODE_H_
