#include "Visitor.h"

void ASTVisitor::visit(const ProcedureTNode* node, std::string& key) {
    std::string isKey = "true";
    std::string isNotKey = "";
//    node->leftChild->accept(this, isKey);
//    node->rightChild->accept(this, isNotKey);
}

void ASTVisitor::visit(const AssignTNode* node, std::string& key) {
    std::string isKey = "true";
    std::string isNotKey = "";
    node->leftChild->accept(this, isKey);
    node->rightChild->accept(this, isNotKey);
}

void ASTVisitor::visit(const VariableTNode* node, std::string& key) {
    if (key == "true") {
        currKey = node->content;
    }

    variablesHashset.insert(node->content);

    if (!currKey.empty()) {
        std::unordered_set<std::string>& set = assignVarHashmap[currKey];
        set.insert(node->content);
    }
}

void ASTVisitor::visit(const ConstantTNode* node, std::string& key) {
    if (key == "true") {
        currKey = node->content;
    } else {
        constantsHashset.insert(node->content);

        if (!currKey.empty()) {
            std::unordered_set<std::string>& set = assignConstHashmap[currKey];
            set.insert(node->content);
        }
    }
}

void ASTVisitor::visit(const PlusTNode* node, std::string& key) {
    node->leftChild->accept(this, key);
    node->rightChild->accept(this, key);
}

void ASTVisitor::visit(const MinusTNode* node, std::string& key) {
    node->leftChild->accept(this, key);
    node->rightChild->accept(this, key);
}
