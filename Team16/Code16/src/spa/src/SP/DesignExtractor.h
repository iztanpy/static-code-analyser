#ifndef TEAM16_CODE16_SRC_SPA_SRC_SP_DESIGNEXTRACTOR_H_
#define TEAM16_CODE16_SRC_SPA_SRC_SP_DESIGNEXTRACTOR_H_

// C system headers
#include <stdio.h>

// C++ system headers
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>

// Other library or external headers
#include "Visitor.h"
#include "TNode.h"

class TNode;
class ASTVisitor;

class DesignExtractor {
 public:
    DesignExtractor() = default;
    int extractDesign(std::shared_ptr<TNode> root, ASTVisitor* visitor);
    int populatePKB();
};

#endif  // TEAM16_CODE16_SRC_SPA_SRC_SP_DESIGNEXTRACTOR_H_
