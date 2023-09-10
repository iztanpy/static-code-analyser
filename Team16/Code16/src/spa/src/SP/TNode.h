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
  virtual ~TNode() = default;
  TokenType type = TokenType::kUnknownTokenType;
  std::string content = "";
  std::shared_ptr<TNode> leftChild;
  std::shared_ptr<TNode> rightChild;
  virtual void accept(ASTVisitor* visitor, std::string& key) const = 0;
  void addChild(const std::shared_ptr<TNode>& child) {
      if (!leftChild) {
          std::cout << "TNode addLeftChild with content: " << child->content << std::endl;
          leftChild = child;
      } else if (!rightChild) {
          std::cout << "TNode addRightChild with content: " << child->content << std::endl;
          rightChild = child;
      } else {
          std::cout << "Error: TNode already has two children" << std::endl;
          throw std::invalid_argument("Error: TNode already has two children");
      }
  }

  bool operator==(const TNode& other) const {
      std::cout << "TNode operator== called" << std::endl;
      return type == other.type && content == other.content;
  }
  std::string getContent() const {
      return content;
  }
  void print() const {
      std::cout << "TNode type" << std::endl;
  }
};

class AssignTNode : public TNode {
 public:
  AssignTNode() {
      std::cout << "assign node created" << std::endl;
      type = TokenType::kEntityAssign;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
  void print() const {
      std::cout << "AssignTNode type" << std::endl;
  }
};

class VariableTNode : public TNode {
 public:
  explicit VariableTNode(const std::string& c) {
      std::cout << "variable node created with content: " << content << std::endl;
      type = TokenType::kLiteralName;
      content = c;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
  void print() const {
      std::cout << "VariableTNode type" << std::endl;
  }
};

class ConstantTNode : public TNode {
 public:
  explicit ConstantTNode(const std::string& c) {
      std::cout << "constant node created with content: " << content << std::endl;
      type = TokenType::kLiteralInteger;
      content = c;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
  void print() const {
      std::cout << "ConstantTNode type" << std::endl;
  }
};

class PlusTNode : public TNode {
 public:
  PlusTNode() {
      std::cout << "plus node created" << std::endl;
      type = TokenType::kOperatorPlus;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
  void print() const {
      std::cout << "PlusTNode type" << std::endl;
  }
};

class MinusTNode : public TNode {
 public:
  MinusTNode() {
      std::cout << "minus node created" << std::endl;
      type = TokenType::kOperatorMinus;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
  void print() const {
      std::cout << "MinusTNode type" << std::endl;
  }
};

class TNodeFactory {
 public:
  static std::shared_ptr<TNode> createNode(const Token& token) {
      switch (token.tokenType) {
          case TokenType::kEntityAssign:std::cout << "create assign node " << std::endl;
              return std::make_shared<AssignTNode>();
          case TokenType::kLiteralName: {
              std::string c = token.value;
              return std::make_shared<VariableTNode>(c);
          }
          case TokenType::kLiteralInteger: {
              std::string c = token.value;
              return std::make_shared<ConstantTNode>(c);
          }
          case TokenType::kOperatorPlus:std::cout << "create plus node " << std::endl;
              return std::make_shared<PlusTNode>();
          case TokenType::kOperatorMinus:std::cout << "create minus node " << std::endl;
              return std::make_shared<MinusTNode>();
          default:std::cout << "Error: unknown token type" << std::endl;
              throw std::invalid_argument("Error: unknown token type");
      }
  }
};

#endif  // TEAM16_CODE16_SRC_SPA_SRC_SP_TNODE_H_
