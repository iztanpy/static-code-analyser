#pragma once

#ifndef SPA_VISITOR_H
#define SPA_VISITOR_H
#include "TNode.h"

class TNode;
enum class NodeType;

class Visitor {
public:
    virtual void visit(const TNode *node) const = 0;
};

class ASTVisitor: public Visitor {
public:
    ASTVisitor() = default;

    void visit(const TNode* node) const override;
};
#endif //SPA_VISITOR_H
