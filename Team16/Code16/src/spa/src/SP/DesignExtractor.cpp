#include "SP/DesignExtractor.h"

int DesignExtractor::extractDesign(std::shared_ptr<TNode> root, ASTVisitor* visitor) {
    bool isRightType = root->type == TokenType::kOperatorEqual;
    std::string key;
    root->accept(visitor, key);
    return 0;
}

int DesignExtractor::populatePKB() {
    return 0;
}
