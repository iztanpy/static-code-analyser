#include "Visitor.h"

void ASTVisitor::visit(const ProcedureTNode* node, std::string& key) {}

void ASTVisitor::visit(const AssignTNode* node, std::string& key) {
    std::string isLHS = "true";
    std::string isNotLHS;
    node->leftChild->accept(this, isLHS);
    node->rightChild->accept(this, isNotLHS);
}

void ASTVisitor::visit(const VariableTNode* node, std::string& key) {
    // If var is on LHS of assign, then it is a key
    if (key == "true") {
        currKey = node->content;
        usesLineLHSMap.insert({node->statementNumber, node->content});
    } else {
        std::unordered_set<std::string>& setVar = usesLineRHSVarMap[node->statementNumber];
        setVar.insert(node->content);
        std::unordered_set<std::string>& set = usesLineRHSPatternMap[node->statementNumber];
        set.insert(node->content);
    }
    variables.insert(node->content);
}

void ASTVisitor::visit(const ConstantTNode* node, std::string& key) {
    constants.insert(node->content);
    std::unordered_set<std::string>& set = usesLineRHSPatternMap[node->statementNumber];
    set.insert(node->content);
}

void ASTVisitor::visit(const PlusTNode* node, std::string& key) {
    node->leftChild->accept(this, key);
    node->rightChild->accept(this, key);
    std::unordered_set<std::string>& set = usesLineRHSPatternMap[node->statementNumber];
    set.insert(node->getContent());
}

void ASTVisitor::visit(const MinusTNode* node, std::string& key) {
    node->leftChild->accept(this, key);
    node->rightChild->accept(this, key);
    std::unordered_set<std::string>& set = usesLineRHSPatternMap[node->statementNumber];
    set.insert(node->getContent());
}

void ASTVisitor::visit(const ReadTNode* node, std::string& key) {
    node->leftChild->accept(this, key);
    node->rightChild->accept(this, key);
//    std::unordered_set<std::string>& set = usesLineRHSPatternMap[node->statementNumber];
//    set.insert(node->getContent());
}
