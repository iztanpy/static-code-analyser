#include "SP/DesignExtractor.h"

int DesignExtractor::extractDesign(std::shared_ptr<TNode> root, ASTVisitor* visitor) {
    std::string key;
    root->accept(visitor, key);
    return 0;
}
