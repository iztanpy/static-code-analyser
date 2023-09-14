#include "TNode.h"
#include "Visitor.h"

TNode::TNode(int statementNumber) : statementNumber(statementNumber) {}

void AssignTNode::accept(ASTVisitor *visitor, std::string& key) const {
    std::cout << "AssignTNode node accept visitor" << std::endl;
    visitor->visit(this, key);
}

void VariableTNode::accept(ASTVisitor *visitor, std::string& key) const {
    std::cout << "VariableTNode node accept visitor" << std::endl;
    visitor->visit(this, key);
}

void ConstantTNode::accept(ASTVisitor *visitor, std::string& key) const {
    std::cout << "ConstantTNode node accept visitor" << std::endl;
    visitor->visit(this, key);
}

void PlusTNode::accept(ASTVisitor *visitor, std::string& key) const {
    std::cout << "PlusTNode node accept visitor" << std::endl;
    visitor->visit(this, key);
}

void MinusTNode::accept(ASTVisitor *visitor, std::string& key) const {
    std::cout << "PlusTNode node accept visitor" << std::endl;
    visitor->visit(this, key);
}
