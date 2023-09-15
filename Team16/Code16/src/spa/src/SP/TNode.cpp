#include "TNode.h"
#include "Visitor.h"

TNode::TNode(int statementNumber) : statementNumber(statementNumber) {}

void ProcedureTNode::accept(ASTVisitor *visitor, std::string& key) const {
    visitor->visit(this, key);
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
