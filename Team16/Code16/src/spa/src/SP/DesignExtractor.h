#ifndef SPA_DESIGNEXTRACTOR_H
#define SPA_DESIGNEXTRACTOR_H
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "Visitor.h"

using namespace std;

class TNode;
class ASTVisitor;

class DesignExtractor {
public:
    DesignExtractor() = default;
    int extractDesign(std::shared_ptr<TNode> root, ASTVisitor* visitor);
    int populatePKB();
};

#endif //SPA_DESIGNEXTRACTOR_H
