#include "TNode.h"
#include "Visitor.h"

TNode::TNode(int statementNumber) : statementNumber(statementNumber) {}

void ProcedureTNode::setEndStatementNumber(int statementNumber) {
  if (endStatementNumber == -1) {
    endStatementNumber = statementNumber;
  }
}

int ProcedureTNode::getStartStatementNumber() const {
  return startStatementNumber;
}

void ProcedureTNode::accept(ASTVisitor *visitor, std::string& key) const {
    visitor->visit(this, key);
}

void AssignTNode::setFullRHS(const std::string& rhs) {
    fullRHS = rhs;
}

std::string AssignTNode::getFullRHS() const {
    return fullRHS;
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
