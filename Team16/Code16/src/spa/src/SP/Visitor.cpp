#include "Visitor.h"

void ASTVisitor::visit(const ProcedureTNode* node, std::string& key) {
    std::set<std::string> procedureLabels = getProcedureLabels();
    auto existingLabel = procedureLabels.find(node->getContent());
    if (existingLabel != procedureLabels.end()) {
        std::cout << node->getContent() << std::endl;
        throw std::runtime_error("Semantic error! We don't support anything and everything.");
        //throw InvalidSemanticError();
    } else {
        insertProcedureLabel(node->getContent());
    }
}

void ASTVisitor::visit(const AssignTNode* node, std::string& key) {
    std::string isLHS = "true";
    std::string isNotLHS;
    node->leftChild->accept(this, isLHS);
    node->rightChild->accept(this, isNotLHS);
    statementTypesMap.insert({node->statementNumber, StmtEntity::kAssign});
}

void ASTVisitor::visit(const VariableTNode* node, std::string& key) {
    // If var is on LHS of assign, then it is a key
    if (key == "true") {
        currKey = node->content;
        usesLineLHSMap.insert({node->statementNumber, node->content});
        modifiesMap.insert({node->statementNumber, node->getContent()});
    } else {
        std::unordered_set<std::string>& setVar = usesLineRHSVarMap[node->statementNumber];
        setVar.insert(node->content);
        if (key != "not pattern") {
          std::unordered_set<std::string>& set = usesLineRHSPatternMap[node->statementNumber];
          set.insert(node->content);
        }
    }
    variables.insert(node->content);
}

void ASTVisitor::visit(const ConstantTNode* node, std::string& key) {
    constants.insert(node->content);
    if (key != "not pattern") {
      std::unordered_set<std::string>& set = usesLineRHSPatternMap[node->statementNumber];
      set.insert(node->content);
    }
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

void ASTVisitor::visit(const MultiplyTNode* node, std::string& key) {
  node->leftChild->accept(this, key);
  node->rightChild->accept(this, key);
  std::unordered_set<std::string>& set = usesLineRHSPatternMap[node->statementNumber];
  set.insert(node->getContent());
}

void ASTVisitor::visit(const DivideTNode* node, std::string& key) {
  node->leftChild->accept(this, key);
  node->rightChild->accept(this, key);
  std::unordered_set<std::string>& set = usesLineRHSPatternMap[node->statementNumber];
  set.insert(node->getContent());
}

void ASTVisitor::visit(const ModTNode* node, std::string& key) {
  node->leftChild->accept(this, key);
  node->rightChild->accept(this, key);
  std::unordered_set<std::string>& set = usesLineRHSPatternMap[node->statementNumber];
  set.insert(node->getContent());
}

void ASTVisitor::visit(const CondOperatorTNode* node, std::string& key) {
  std::string notPattern = "not pattern";
  node->leftChild->accept(this, notPattern);
  if (node->rightChild) {
    node->rightChild->accept(this, notPattern);
  }
//  std::unordered_set<std::string>& set = usesLineRHSPatternMap[node->statementNumber];
//  set.insert(node->getContent());
}

void ASTVisitor::visit(const RelOperatorTNode* node, std::string& key) {
  std::string notPattern = "not pattern";
  node->leftChild->accept(this, notPattern);
  node->rightChild->accept(this, notPattern);
//  std::unordered_set<std::string>& set = usesLineRHSPatternMap[node->statementNumber];
//  set.insert(node->getContent());
}

void ASTVisitor::visit(const ReadTNode* node, std::string& key) {
    variables.insert(node->getContent());
    statementTypesMap.insert({node->statementNumber, StmtEntity::kRead});
    modifiesMap.insert({node->statementNumber, node->getContent()});
}

void ASTVisitor::visit(const WhileTNode* node, std::string& key) {
    node->leftChild->accept(this, key);
    statementTypesMap.insert({node->statementNumber, StmtEntity::kWhile});
}

void ASTVisitor::visit(const PrintTNode* node, std::string& key) {
    std::unordered_set<std::string>& set = usesLineRHSVarMap[node->statementNumber];
    set.insert(node->content);
    variables.insert(node->getContent());
    statementTypesMap.insert({node->statementNumber, StmtEntity::kPrint});
}

void ASTVisitor::visit(const IfTNode* node, std::string& key) {
    node->leftChild->accept(this, key);
    statementTypesMap.insert({node->statementNumber, StmtEntity::kIf});
}

void ASTVisitor::visit(const CallTNode* node, std::string& key) {
    statementTypesMap.insert({node->statementNumber, StmtEntity::kCall});
}




