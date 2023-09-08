#include "Visitor.h"

void ASTVisitor::visit(const TNode* node) const {
    std::cout << "Visiting variable ast node: " << node->type << " " << node->content << std::endl;
};

//void ASTVisitor::visit(const TNode* node, NodeType type) const {
//    std::cout << "Visiting constant ast node: " << node->type << " " << node->content << std::endl;
//};

