#include "SP/visitor.h"

void ASTVisitor::visit(const ProcedureTNode* node, std::string& key) {
  std::set < std::string > procedureLabels = getProcedureLabels();
  auto existingLabel = procedureLabels.find(node->getContent());
  if (existingLabel != procedureLabels.end()) {
    throw InvalidSemanticError();
  } else {
    insertProcedureLabel(node->getContent());
  }
  procedureLineNumberHashmap[node->getContent()].first = node->getStartStatementNumber();
}

void ASTVisitor::visit(const AssignTNode* node, std::string& key) {
  std::string isLHS = "true";
  std::string isNotLHS;
  node->acceptLeftChild(this, isLHS);
  node->acceptRightChild(this, isNotLHS);
  statementTypesMap.insert({node->getStatementNumber(), StmtEntity::kAssign});
  assignLineFullRHSMap.insert({node->getStatementNumber(), node->getFullRHS()});
}

void ASTVisitor::visit(const VariableTNode* node, std::string& key) {
  // If var is on LHS of assign, then it is a key
  if (key == "true") {
    usesLineLHSMap.insert({node->getStatementNumber(), node->getContent()});
    modifiesMap.insert({node->getStatementNumber(), node->getContent()});
  } else {
    std::unordered_set<std::string>& setVar = usesLineRHSVarMap[node->getStatementNumber()];
    setVar.insert(node->getContent());
    if (key == "controlWhile") {
      std::unordered_set<std::string>& set = whileControlVarMap[node->getStatementNumber()];
      set.insert(node->getContent());
    } else if (key == "controlIf") {
      std::unordered_set<std::string>& set = ifControlVarMap[node->getStatementNumber()];
      set.insert(node->getContent());
    } else {
      std::unordered_set<std::string>& set = assignLinePartialRHSPatternMap[node->getStatementNumber()];
      set.insert(node->getContent(true));
    }
  }
  variables.insert(node->getContent());
}

void ASTVisitor::visit(const ConstantTNode* node, std::string& key) {
  constants.insert(node->getContent());
  if (key.empty()) {
    std::unordered_set<std::string>& set = assignLinePartialRHSPatternMap[node->getStatementNumber()];
    set.insert(node->getContent(true));
  }
}

void ASTVisitor::visit(const PlusTNode* node, std::string& key) {
  node->acceptLeftChild(this, key);
  node->acceptRightChild(this, key);
  if (key.empty()) {
    std::unordered_set<std::string>& set = assignLinePartialRHSPatternMap[node->getStatementNumber()];
    set.insert(node->getContent(true));
  }
}

void ASTVisitor::visit(const MinusTNode* node, std::string& key) {
  node->acceptLeftChild(this, key);
  node->acceptRightChild(this, key);
  if (key.empty()) {
    std::unordered_set<std::string>& set = assignLinePartialRHSPatternMap[node->getStatementNumber()];
    set.insert(node->getContent(true));
  }
}

void ASTVisitor::visit(const MultiplyTNode* node, std::string& key) {
  node->acceptLeftChild(this, key);
  node->acceptRightChild(this, key);
  if (key.empty()) {
    std::unordered_set<std::string>& set = assignLinePartialRHSPatternMap[node->getStatementNumber()];
    set.insert(node->getContent(true));
  }
}

void ASTVisitor::visit(const DivideTNode* node, std::string& key) {
  node->acceptLeftChild(this, key);
  node->acceptRightChild(this, key);
  if (key.empty()) {
    std::unordered_set<std::string>& set = assignLinePartialRHSPatternMap[node->getStatementNumber()];
    set.insert(node->getContent(true));
  }
}

void ASTVisitor::visit(const ModTNode* node, std::string& key) {
  node->acceptLeftChild(this, key);
  node->acceptRightChild(this, key);
  if (key.empty()) {
    std::unordered_set<std::string>& set = assignLinePartialRHSPatternMap[node->getStatementNumber()];
    set.insert(node->getContent(true));
  }
}

void ASTVisitor::visit(const CondOperatorTNode* node, std::string& key) {
  node->acceptLeftChild(this, key);
  node->acceptRightChild(this, key);
}

void ASTVisitor::visit(const RelOperatorTNode* node, std::string& key) {
  node->acceptLeftChild(this, key);
  node->acceptRightChild(this, key);
}

void ASTVisitor::visit(const ReadTNode* node, std::string& key) {
  variables.insert(node->getContent());
  statementTypesMap.insert({node->getStatementNumber(), StmtEntity::kRead});
  modifiesMap.insert({node->getStatementNumber(), node->getContent()});
}

void ASTVisitor::visit(const WhileTNode* node, std::string& key) {
  std::string controlWhile = "controlWhile";
  node->acceptLeftChild(this, controlWhile);
  statementTypesMap.insert({node->getStatementNumber(), StmtEntity::kWhile});
}

void ASTVisitor::visit(const PrintTNode* node, std::string& key) {
  std::unordered_set<std::string>& set = usesLineRHSVarMap[node->getStatementNumber()];
  set.insert(node->getContent());
  variables.insert(node->getContent());
  statementTypesMap.insert({node->getStatementNumber(), StmtEntity::kPrint});
}

void ASTVisitor::visit(const IfTNode* node, std::string& key) {
  std::string controlIf = "controlIf";
  node->acceptLeftChild(this, controlIf);
  statementTypesMap.insert({node->getStatementNumber(), StmtEntity::kIf});
}

void ASTVisitor::visit(const CallTNode* node, std::string& key) {
  statementTypesMap.insert({node->getStatementNumber(), StmtEntity::kCall});
}
