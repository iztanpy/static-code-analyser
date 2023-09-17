#include "Visitor.h"

void ASTVisitor::visit(const ProcedureTNode* node, std::string& key) {}

void ASTVisitor::visit(const AssignTNode* node, std::string& key) {
    std::string isKey = "true";
    std::string isNotKey = "";
    node->leftChild->accept(this, isKey);
    node->rightChild->accept(this, isNotKey);
    assignmentStatementsHashset.insert(node->statementNumber);
}

void ASTVisitor::visit(const VariableTNode* node, std::string& key) {
    // If var is on LHS of assign, then it is a key
    if (key == "true") {
        currKey = node->content;
        usesStatementNumberVarHashmap.insert({node->statementNumber, node->content});
    } else {
        std::unordered_set<std::string>& setVar = varUsesMap[node->statementNumber];
        setVar.insert(node->content);
        std::unordered_set<std::string>& set = usesStatementNumberHashmap[node->statementNumber];
        set.insert(node->content);
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

    std::unordered_set<std::string>& set = usesStatementNumberHashmap[node->statementNumber];
    set.insert(node->content);
    std::unordered_set<std::string>& setConst = constUsesMap[node->statementNumber];
    setConst.insert(node->content);
}

void ASTVisitor::visit(const PlusTNode* node, std::string& key) {
    node->leftChild->accept(this, key);
    node->rightChild->accept(this, key);
    std::unordered_set<std::string>& set = usesStatementNumberHashmap[node->statementNumber];
    set.insert(node->getContent());
}

void ASTVisitor::visit(const MinusTNode* node, std::string& key) {
    node->leftChild->accept(this, key);
    node->rightChild->accept(this, key);
    std::unordered_set<std::string>& set = usesStatementNumberHashmap[node->statementNumber];
    set.insert(node->getContent());
}
