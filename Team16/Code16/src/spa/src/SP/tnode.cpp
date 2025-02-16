#include "SP/tnode.h"
#include "SP/visitor.h"

TNode::TNode(int statementNumber) : statementNumber(statementNumber) {}

void TNode::acceptLeftChild(ASTVisitor* visitor, std::string& key) const {
  leftChild->accept(visitor, key);
}

void TNode::acceptRightChild(ASTVisitor* visitor, std::string& key) const {
  if (rightChild) {
    rightChild->accept(visitor, key);
  }
}

int ProcedureTNode::getStartStatementNumber() const {
  return startStatementNumber;
}

void ProcedureTNode::accept(ASTVisitor *visitor, std::string& key) const {
  visitor->visit(this, key);
}

std::string AssignTNode::getFullRHS() const {
  return rightChild->getContent(true);
}

void AssignTNode::accept(ASTVisitor *visitor, std::string& key) const {
  visitor->visit(this, key);
}

void VariableTNode::accept(ASTVisitor *visitor, std::string& key) const {
  visitor->visit(this, key);
}

void ConstantTNode::accept(ASTVisitor *visitor, std::string& key) const {
  visitor->visit(this, key);
}

void PlusTNode::accept(ASTVisitor *visitor, std::string& key) const {
  visitor->visit(this, key);
}

void MinusTNode::accept(ASTVisitor *visitor, std::string& key) const {
  visitor->visit(this, key);
}

void MultiplyTNode::accept(ASTVisitor *visitor, std::string& key) const {
  visitor->visit(this, key);
}

void DivideTNode::accept(ASTVisitor *visitor, std::string& key) const {
  visitor->visit(this, key);
}

void ModTNode::accept(ASTVisitor *visitor, std::string& key) const {
  visitor->visit(this, key);
}

void RelOperatorTNode::accept(ASTVisitor* visitor, std::string& key) const {
  visitor->visit(this, key);
}

void CondOperatorTNode::accept(ASTVisitor* visitor, std::string& key) const {
  visitor->visit(this, key);
}

void ReadTNode::accept(ASTVisitor* visitor, std::string& key) const {
  visitor->visit(this, key);
}

void WhileTNode::accept(ASTVisitor* visitor, std::string& key) const {
  visitor->visit(this, key);
}

void PrintTNode::accept(ASTVisitor* visitor, std::string& key) const {
  visitor->visit(this, key);
}

void IfTNode::accept(ASTVisitor* visitor, std::string& key) const {
  visitor->visit(this, key);
}

void CallTNode::accept(ASTVisitor* visitor, std::string& key) const {
  visitor->visit(this, key);
}
