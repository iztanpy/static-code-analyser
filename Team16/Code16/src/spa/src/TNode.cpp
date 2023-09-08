#include "TNode.h"

void TNode::accept(ASTVisitor *visitor) const {
    visitor->visit(this);
}

void TNode::addChild(const TNode &child) {
    children.emplace_back(child);
}

//NodeType TNode::getNodeType(std::string tokenType) {
//    if (tokenType == "variable") {
//        return variable;
//    } else if (tokenType == "constant") {
//        return constant;
//    } else if (tokenType == "plus" || tokenType == "minus") {
//        return assign;
//    }
//    // change to throw error
//    return assign;
//}
