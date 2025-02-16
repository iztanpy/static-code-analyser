#pragma once

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "sp_tokeniser/token.h"
#include "utils/error.h"

class ASTVisitor;
enum class TokenType;


/**
 * @class TNode
 * @brief Base class for representing nodes in the Abstract Syntax Tree (AST).
 *
 * The `TNode` class serves as the base class for representing nodes in the Abstract Syntax Tree (AST).
 * It provides common attributes and methods for AST nodes.
 */
class TNode {
 protected:
  int statementNumber = 0;
  TokenType type = TokenType::kUnknownTokenType;
  std::string content = "";
  std::shared_ptr<TNode> leftChild;
  std::shared_ptr<TNode> rightChild;

 public:
  /**
   * @brief Constructs a TNode object with a statement number.
   * @param statementNumber The statement number associated with the node.
   */    explicit TNode(int statementNumber);
  virtual ~TNode() = default;
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  virtual void accept(ASTVisitor* visitor, std::string& key) const = 0;
  /**
    * @brief Accepts an ASTVisitor for visiting the left child node.
    * @param visitor A pointer to the ASTVisitor.
    * @param key A string key used for the visitation.
   */
  void acceptLeftChild(ASTVisitor* visitor, std::string& key) const;
  /**
    * @brief Accepts an ASTVisitor for visiting the right child node.
    * @param visitor A pointer to the ASTVisitor.
    * @param key A string key used for the visitation.
   */
  void acceptRightChild(ASTVisitor* visitor, std::string& key) const;
  /**
    * @brief Adds a child node to current node.
    * @param child A child node.
    */
  void addChild(const std::shared_ptr<TNode>& child) {
    if (!leftChild) {
      leftChild = child;
    } else if (!rightChild) {
      rightChild = child;
    } else {
      throw std::invalid_argument("Error: TNode already has two children");
    }
  }
  /**
    * @brief Checks for equality of TNode objects.
    * @param other A TNode object to be compared with current node.
   */
  bool operator==(const TNode& other) const {
    return type == other.type && content == other.content;
  }
  /**
   * @brief Gets the content of the node as a string.
   * @param withBrackets A boolean indicating whether to include brackets in the string representation.
   * @return The content of the node.
   */
  virtual std::string getContent(const bool withBrackets = false) const {
    return content;
  }
  /**
   * @brief Gets the statement number of the node.
   * @return The statement number of the node.
   */
  int getStatementNumber() const {
    return statementNumber;
  }
  /**
   * @brief Gets the type of the node.
   * @return The type of the node.
   */
  TokenType getType() const {
    return type;
  }
};

/**
 * @class ProcedureTNode
 * @brief Represents a procedure node in the AST.
 *
 * The `ProcedureTNode` class represents a procedure node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for procedure nodes.
 */
class ProcedureTNode : public TNode {
 private:
  int startStatementNumber = -1;
  int endStatementNumber = -1;

 public:
  /**
   * @brief Constructs a ProcedureTNode object with a procedure name.
   * @param procedureName The name of the procedure.
  */
  explicit ProcedureTNode(const std::string& procedureName, int startStatementNumber) : TNode(0) {
      type = TokenType::kEntityProcedure;
      content = procedureName;
      this->startStatementNumber = startStatementNumber;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
  */
  void accept(ASTVisitor* visitor, std::string& key) const override;
  /**
   * @brief Gets starting statement number of procedure.
   * @return An int representing statement number.
  */
  int getStartStatementNumber() const;
};

/**
 * @class ReadTNode
 * @brief Represents a procedure node in the AST.
 *
 * The `ReadTNode` class represents a read node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for read nodes.
 */
class ReadTNode : public TNode {
 public:
  /**
  * @brief Constructs a ReadTNode object with a statement number and content.
  * @param statementNumber The statement number of the read statement.
  * @param c The content of the read statement.
  */
  explicit ReadTNode(int statementNumber, const std::string& c) : TNode(statementNumber) {
  type = TokenType::kEntityRead;
  content = c;
  }
  /**
  * @brief Accepts an ASTVisitor for visiting the node.
  * @param visitor A pointer to the ASTVisitor.
  * @param key A string key used for the visitation.
  */
  void accept(ASTVisitor* visitor, std::string& key) const override;
};

/**
 * @class AssignTNode
 * @brief Represents an assignment statement node in the AST.
 *
 * The `AssignTNode` class represents an assignment statement node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for assignment statements.
 */
class AssignTNode : public TNode {
 private:
  std::string fullRHS = "";

 public:
  explicit AssignTNode(int statementNumber) : TNode(statementNumber) {
      type = TokenType::kEntityAssign;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
  /**
   * @brief Gets the right-hand-side expression of an assign statement.
   * @return the full right-hand-side expression of an assign statement.
   */
  std::string getFullRHS() const;
};

/**
 * @class VariableTNode
 * @brief Represents a variable node in the AST.
 *
 * The `VariableTNode` class represents a variable node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for variable nodes.
 */
class VariableTNode : public TNode {
 public:
  explicit VariableTNode(int statementNumber, const std::string& c) : TNode(statementNumber) {
    type = TokenType::kLiteralName;
    content = c;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
  /**
   * @brief Gets the content of the TNode.
   * @param withBrackets A boolean indicating if brackets should be included in the returned value.
   * @return A string representing content of TNode.
   */
  std::string getContent(bool withBrackets = false) const override {
    if (withBrackets) {
      return "(" + content + ")";
    }
    return content;
  }
};

/**
 * @class ConstantTNode
 * @brief Represents a constant node in the AST.
 *
 * The `ConstantTNode` class represents a constant node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for constant nodes.
 */
class ConstantTNode : public TNode {
 public:
  explicit ConstantTNode(int statementNumber, const std::string& c) : TNode(statementNumber) {
    type = TokenType::kLiteralInteger;
    content = c;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
  /**
   * @brief Gets the content of the TNode.
   * @param withBrackets A boolean indicating if brackets should be included in the returned value.
   * @return A string representing content of TNode.
   */
  std::string getContent(bool withBrackets = false) const override {
    if (withBrackets) {
      return "(" + content + ")";
    }
    return content;
  }
};

/**
 * @class PlusTNode
 * @brief Represents an addition operation node in the AST.
 *
 * The `PlusTNode` class represents an addition operation node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for addition operations.
 */
class PlusTNode : public TNode {
 public:
  explicit PlusTNode(int statementNumber) : TNode(statementNumber) {
    type = TokenType::kOperatorPlus;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
  /**
   * @brief Gets the content of the TNode.
   * @param withBrackets A boolean indicating if brackets should be included in the returned value.
   * @return A string representing content of TNode.
   */
  std::string getContent(bool withBrackets) const override {
    if (withBrackets) {
      return "(" + leftChild->getContent(withBrackets) + "+" + rightChild->getContent(withBrackets) + ")";
    }
    return leftChild->getContent(withBrackets) + "+" + rightChild->getContent(withBrackets);
  }
};

/**
 * @class MinusTNode
 * @brief Represents a subtraction operation node in the AST.
 *
 * The `MinusTNode` class represents a subtraction operation node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for subtraction operations.
 */
class MinusTNode : public TNode {
 public:
  explicit MinusTNode(int statementNumber) : TNode(statementNumber) {
    type = TokenType::kOperatorMinus;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
  /**
   * @brief Gets the content of the TNode.
   * @param withBrackets A boolean indicating if brackets should be included in the returned value.
   * @return A string representing content of TNode.
   */
  std::string getContent(bool withBrackets) const override {
    if (withBrackets) {
      return "(" + leftChild->getContent(withBrackets) + "-" + rightChild->getContent(withBrackets) + ")";
    }
    return leftChild->getContent(withBrackets) + "-" + rightChild->getContent(withBrackets);
  }
};


/**
 * @class WhileTNode
 * @brief Represents a while statement node in the AST.
 *
 * The `WhileTNode` class represents a while statement node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for while statements.
 */
class WhileTNode : public TNode {
 public:
  explicit WhileTNode(int statementNumber) : TNode(statementNumber) {
    type = TokenType::kEntityWhile;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
};

/**
 * @class PrintTNode
 * @brief Represents a print statement node in the AST.
 *
 * The `PrintTNode` class represents a print statement node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for print statements.
 */
class PrintTNode : public TNode {
 public:
  explicit PrintTNode(int statementNumber, const std::string& c) : TNode(statementNumber) {
    type = TokenType::kEntityPrint;
    content = c;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
};

/**
 * @class IfTNode
 * @brief Represents an if statement node in the AST.
 *
 * The `IfTNode` class represents an if statement node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for if statements.
 */
class IfTNode : public TNode {
 public:
  explicit IfTNode(int statementNumber) : TNode(statementNumber) {
    type = TokenType::kEntityIf;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
};

/**
 * @class CallTNode
 * @brief Represents a procedure call node in the AST.
 *
 * The `CallTNode` class represents a procedure call node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for procedure calls.
 */
class CallTNode : public TNode {
 public:
  explicit CallTNode(int statementNumber, const std::string& c) : TNode(statementNumber) {
    type = TokenType::kEntityCall;
    content = c;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
};

/**
 * @class MultiplyTNode
 * @brief Represents a multiplication operation node in the AST.
 *
 * The `MultiplyTNode` class represents a multiplication operation node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for multiplication operations.
 */
class MultiplyTNode : public TNode {
 public:
  explicit MultiplyTNode(int statementNumber) : TNode(statementNumber) {
    type = TokenType::kOperatorMultiply;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
  /**
   * @brief Gets the content of the TNode.
   * @param withBrackets A boolean indicating if brackets should be included in the returned value.
   * @return A string representing content of TNode.
   */
  std::string getContent(bool withBrackets) const override {
    if (withBrackets) {
      return "(" + leftChild->getContent(withBrackets) + "*" + rightChild->getContent(withBrackets) + ")";
    }
    return leftChild->getContent(withBrackets) + "*" + rightChild->getContent(withBrackets);
  }
};

/**
 * @class DivideTNode
 * @brief Represents a division operation node in the AST.
 *
 * The `DivideTNode` class represents a division operation node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for division operations.
 */
class DivideTNode : public TNode {
 public:
  explicit DivideTNode(int statementNumber) : TNode(statementNumber) {
    type = TokenType::kOperatorDivide;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
  /**
   * @brief Gets the content of the TNode.
   * @param withBrackets A boolean indicating if brackets should be included in the returned value.
   * @return A string representing content of TNode.
   */
  std::string getContent(bool withBrackets) const override {
    if (withBrackets) {
      return "(" + leftChild->getContent(withBrackets) + "/" + rightChild->getContent(withBrackets) + ")";
    }
    return leftChild->getContent(withBrackets) + "/" + rightChild->getContent(withBrackets);
  }
};

/**
 * @class ModTNode
 * @brief Represents a modulo operation node in the AST.
 *
 * The `ModTNode` class represents a modulo operation node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for modulo operations.
 */
class ModTNode : public TNode {
 public:
  explicit ModTNode(int statementNumber) : TNode(statementNumber) {
    type = TokenType::kOperatorMod;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
  /**
   * @brief Gets the content of the TNode.
   * @param withBrackets A boolean indicating if brackets should be included in the returned value.
   * @return A string representing content of TNode.
   */
  std::string getContent(bool withBrackets) const override {
    if (withBrackets) {
      return "(" + leftChild->getContent(withBrackets) + "%" + rightChild->getContent(withBrackets) + ")";
    }
    return leftChild->getContent(withBrackets) + "%" + rightChild->getContent(withBrackets);
  }
};

/**
 * @class RelOperatorTNode
 * @brief Represents a relational operator node in the AST.
 *
 * The `RelOperatorTNode` class represents a relational operator node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for relational operators.
 */
class RelOperatorTNode : public TNode {
 public:
  explicit RelOperatorTNode(int statementNumber, TokenType tokenType) : TNode(statementNumber) {
    type = tokenType;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
  /**
   * @brief Gets the content of the TNode.
   * @param withBrackets A boolean indicating if brackets should be included in the returned value.
   * @return A string representing content of TNode.
   */
  std::string getContent(bool withBrackets = false) const override {
    std::string rep = "";
    switch (type) {
      case TokenType::kOperatorEqual:
        rep = " == ";
        break;
      case TokenType::kOperatorNotEqual:
        rep = " != ";
        break;
      case TokenType::kOperatorGreater:
        rep = " > ";
        break;
      case TokenType::kOperatorLess:
        rep = " < ";
        break;
      case TokenType::kOperatorGreaterEqual:
        rep = " >= ";
        break;
      case TokenType::kOperatorLessEqual:
        rep = " <= ";
        break;
      default:
        throw InvalidTokenTypeError("Error: invalid token type");
    }
    return leftChild->getContent(withBrackets) + rep + rightChild->getContent(withBrackets);
  }
};

/**
 * @class CondOperatorTNode
 * @brief Represents a conditional operator node in the AST.
 *
 * The `CondOperatorTNode` class represents a conditional operator node in the Abstract Syntax Tree (AST).
 * It inherits from the `TNode` base class and provides specialized functionality for conditional operators.
 */
class CondOperatorTNode : public TNode {
 public:
  explicit CondOperatorTNode(int statementNumber, TokenType tokenType) : TNode(statementNumber) {
    type = tokenType;
  }
  /**
   * @brief Accepts an ASTVisitor for visiting the node.
   * @param visitor A pointer to the ASTVisitor.
   * @param key A string key used for the visitation.
   */
  void accept(ASTVisitor* visitor, std::string& key) const override;
  /**
   * @brief Gets the content of the TNode.
   * @param withBrackets A boolean indicating if brackets should be included in the returned value.
   * @return A string representing content of TNode.
   */
  std::string getContent(bool withBrackets = false) const override {
    std::string rep = "";
    switch (type) {
      case TokenType::kOperatorLogicalNot:
        rep = "!";
        break;
      case TokenType::kOperatorLogicalAnd:
        rep = " && ";
        break;
      case TokenType::kOperatorLogicalOr:
        rep = "||";
        break;
      default:
        throw InvalidTokenTypeError("Error: invalid token type");
    }
    if (!leftChild) {
      return rep + "(" + rightChild->getContent(withBrackets) + ")";
    }
    return "(" + leftChild->getContent(withBrackets) + ")" + rep + "(" + rightChild->getContent(withBrackets) + ")";
  }
};

/**
 * @class TNodeFactory
 * @brief A factory class for creating TNode objects.
 *
 * The `TNodeFactory` class is a factory class responsible for creating various types of `TNode` objects
 * based on token types and statement numbers.
 */
class TNodeFactory {
 public:
  /**
  * @brief Creates a TNode object based on a token and statement number.
  * @param token The token representing the node.
  * @param statementNumber The statement number associated with the node.
  * @return A shared pointer to the created TNode object.
  */
  static std::shared_ptr<TNode> createNode(const Token& token, int statementNumber) {
    switch (token.tokenType) {
      case TokenType::kEntityProcedure: {
        return std::make_shared<ProcedureTNode>(token.value, statementNumber);
      }
      case TokenType::kEntityWhile: {
        return std::make_shared<WhileTNode>(statementNumber);
      }
      case TokenType::kEntityPrint: {
        return std::make_shared<PrintTNode>(statementNumber, token.value);
      }
      case TokenType::kEntityCall: {
        return std::make_shared<CallTNode>(statementNumber, token.value);
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
      }
      case TokenType::kOperatorEqual:
      case TokenType::kOperatorNotEqual:
      case TokenType::kOperatorGreater:
      case TokenType::kOperatorLess:
      case TokenType::kOperatorGreaterEqual:
      case TokenType::kOperatorLessEqual: {
        return std::make_shared<RelOperatorTNode>(statementNumber, token.tokenType);
      }
      case TokenType::kOperatorLogicalAnd:
      case TokenType::kOperatorLogicalOr:
      case TokenType::kOperatorLogicalNot: {
        return std::make_shared<CondOperatorTNode>(statementNumber, token.tokenType);
      default:
        throw std::invalid_argument("Error: unknown token type");
      }
    }
  }
};
