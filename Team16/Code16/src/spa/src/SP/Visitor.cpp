#include "Visitor.h"

void ASTVisitor::visit(const AssignTNode* node, std::string& key) {
    std::cout << "Visiting AssignTNode" << std::endl;
    std::string isKey = "true";
    std::string isNotKey = "";
    node->leftChild->accept(this, isKey);
    node->rightChild->accept(this, isNotKey);
}

void ASTVisitor::visit(const VariableTNode* node, std::string& key) {
    std::cout << "Visiting VariableTNode" << std::endl;
    if (key == "true") {
        std::cout << "this node is a key" << std::endl;
        currKey = node->content;
        std::cout << "currKey is now " << currKey << std::endl;
    }

    variablesHashset.insert(node->content);
    std::cout << "inserting {" << node->content << "} into variablesHashset" << std::endl;

    if (!currKey.empty()) {
        std::unordered_set<std::string>& set = assignVarHashmap[currKey];
        set.insert(node->content);
        std::cout << "inserting {" << currKey << " : " << node->content << "} into assignVarHashmap" << std::endl;
    }
}

void ASTVisitor::visit(const ConstantTNode* node, std::string& key) {
    std::cout << "Visiting ConstantTNode" << std::endl;
    if (key == "true") {
        std::cout << "this node is a key" << std::endl;
        currKey = node->content;
        std::cout << "currKey is now " << currKey << std::endl;
    } else {
        constantsHashset.insert(node->content);
        std::cout << "inserting {" << node->content << "} into constantsHashset" << std::endl;

        if (!currKey.empty()) {
            std::unordered_set<std::string>& set = assignConstHashmap[currKey];
            set.insert(node->content);
            std::cout << "inserting {" << currKey << " : " << node->content << "} into assignConstHashmap" << std::endl;
        }
    }
}

void ASTVisitor::visit(const PlusTNode* node, std::string& key) {
    std::cout << "Visiting PlusTNode with key: " << key << std::endl;
    node->leftChild->accept(this, key);
    node->rightChild->accept(this, key);
}

void ASTVisitor::visit(const MinusTNode* node, std::string& key) {
    std::cout << "Visiting MinusTNode with key: " << key << std::endl;
    node->leftChild->accept(this, key);
    node->rightChild->accept(this, key);
}
