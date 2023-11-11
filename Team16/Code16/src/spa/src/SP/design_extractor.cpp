#include "SP/design_extractor.h"

int design_extractor::extractDesign(std::shared_ptr<TNode> root, ASTVisitor* visitor) {
  std::string key;
  root->accept(visitor, key);
  return 0;
}
