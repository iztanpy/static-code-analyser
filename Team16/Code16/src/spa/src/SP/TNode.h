#pragma once

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "sp_tokeniser/Token.h"
#include "utils/Error.h"

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

class ReadTNode : public TNode {
 public:
     explicit ReadTNode(int statementNumber, const std::string& c) : TNode(statementNumber) {
        type = TokenType::kEntityRead;
        content = c;
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

class WhileTNode : public TNode {
 public:
  explicit WhileTNode(int statementNumber) : TNode(statementNumber) {
    type = TokenType::kEntityWhile;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
};

class PrintTNode : public TNode {
 public:
    explicit PrintTNode(int statementNumber, const std::string& c) : TNode(statementNumber) {
        type = TokenType::kEntityPrint;
        content = c;
    }
    void accept(ASTVisitor* visitor, std::string& key) const override;
};

class IfTNode : public TNode {
 public:
    explicit IfTNode(int statementNumber) : TNode(statementNumber) {
        type = TokenType::kEntityIf;
    }
    void accept(ASTVisitor* visitor, std::string& key) const override;
};

class CallTNode : public TNode {
 public:
    explicit CallTNode(int statementNumber, const std::string& c) : TNode(statementNumber) {
        type = TokenType::kEntityCall;
        content = c;
    }
    void accept(ASTVisitor* visitor, std::string& key) const override;
};

class MultiplyTNode : public TNode {
 public:
  explicit MultiplyTNode(int statementNumber) : TNode(statementNumber) {
    type = TokenType::kOperatorMultiply;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
  std::string getContent() const override {
    return leftChild->getContent() + " * " + rightChild->getContent();
  }
};

class DivideTNode : public TNode {
 public:
  explicit DivideTNode(int statementNumber) : TNode(statementNumber) {
    type = TokenType::kOperatorDivide;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
  std::string getContent() const override {
    return leftChild->getContent() + " / " + rightChild->getContent();
  }
};

class ModTNode : public TNode {
 public:
  explicit ModTNode(int statementNumber) : TNode(statementNumber) {
    type = TokenType::kOperatorMod;
  }
  void accept(ASTVisitor* visitor, std::string& key) const override;
  std::string getContent() const override {
    return leftChild->getContent() + " % " + rightChild->getContent();
  }
};

class TNodeFactory {
 public:
     static std::shared_ptr<TNode> createNode(const Token& token, int statementNumber) {
         switch (token.tokenType) {
         case TokenType::kEntityProcedure: {
             return std::make_shared<ProcedureTNode>(token.value);
         }
         case TokenType::kEntityWhile: {
             return std::make_shared<WhileTNode>(statementNumber);  // probably needs more information than this
         }
         case TokenType::kEntityPrint: {
             return std::make_shared<PrintTNode>(statementNumber, token.value);
         }
         case TokenType::kEntityCall: {
             return std::make_shared<PrintTNode>(statementNumber, token.value);
         }
         case TokenType::kEntityIf: {
             return std::make_shared<IfTNode>(statementNumber);
         }
         case TokenType::kEntityRead: {
             return std::make_shared<ReadTNode>(statementNumber, token.value);
         }
         case TokenType::kEntityAssign: {
             return std::make_shared<AssignTNode>(statementNumber);
         }
         case TokenType::kLiteralName: {
             return std::make_shared<VariableTNode>(statementNumber, token.value);
         }
         case TokenType::kLiteralInteger: {
             return std::make_shared<ConstantTNode>(statementNumber, token.value);
         }
         case TokenType::kOperatorPlus: {
             return std::make_shared<PlusTNode>(statementNumber);
         }
         case TokenType::kOperatorMinus: {
            return std::make_shared<MinusTNode>(statementNumber);
         }
         case TokenType::kOperatorMultiply: {
           return std::make_shared<MultiplyTNode>(statementNumber);
         }
         case TokenType::kOperatorDivide: {
           return std::make_shared<DivideTNode>(statementNumber);
         }
         case TokenType::kOperatorMod: {
           return std::make_shared<ModTNode>(statementNumber);
         default:
             throw std::invalid_argument("Error: unknown token type");
         }
         }
     }
};
