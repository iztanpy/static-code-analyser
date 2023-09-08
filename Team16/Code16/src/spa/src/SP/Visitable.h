#pragma once

#ifndef SPA_VISITABLE_H
#define SPA_VISITABLE_H

class ASTVisitor;

class Visitable {
public:
    virtual ~Visitable() = default;
    virtual void accept(ASTVisitor *visitor) const = 0;
};

#endif //SPA_VISITABLE_H
