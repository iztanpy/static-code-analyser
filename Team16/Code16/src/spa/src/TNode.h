#pragma once
#ifndef TEAM16_CODE16_SRC_SPA_SRC_TNODE_H_
#define TEAM16_CODE16_SRC_SPA_SRC_TNODE_H_

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include "SP/Visitable.h"
#include "SP/Visitor.h"

class TNode : public Visitable {
 public:
    TNode() = default;
    TNode(std::string t) : type(std::move(t)), children() {};
    TNode(std::string t, std::string c) : type(std::move(t)), content(std::move(c)), children() {};

    std::string type;
    std::string content;
    std::vector<TNode> children; // left is 0 and right is 1

    void addChild(const TNode& child);
    void accept(ASTVisitor* visitor) const override;
//    NodeType getNodeType(std::string tokenType);
};
#endif  // TEAM16_CODE16_SRC_SPA_SRC_TNODE_H_