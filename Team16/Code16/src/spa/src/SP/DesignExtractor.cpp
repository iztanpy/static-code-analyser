#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

//#include "PKB.h"
#include "SP/DesignExtractor.h"
#include "TNode.h"
#include "Visitor.h"

using namespace std;

int DesignExtractor::extractDesign(std::shared_ptr<TNode> root, ASTVisitor* visitor) {
    bool isRightType = root->type == TokenType::kOperatorEqual;
    std::cout << "root going to accept visitor, type: " << isRightType << std::endl;
    std::string key;
    root->accept(visitor, key);
    std::cout << "root accepted visitor" << std::endl;
    return 0;
}

int DesignExtractor::populatePKB() {
    std::cout << "populating PKB " << std::endl;
    return 0;
}