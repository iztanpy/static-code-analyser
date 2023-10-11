#pragma once

#include <memory>
#include <set>
#include <unordered_set>

class CfgNode {
 private:
  std::unordered_set<std::shared_ptr<CfgNode>> children;
  std::shared_ptr<CfgNode> parentNode = nullptr;
  std::set<int> statementNumberSet;

 public:
  CfgNode() {}

  explicit CfgNode(int statementNo) {
    statementNumberSet.insert(statementNo);
  }

  virtual ~CfgNode() = default;

  void addChildren(const std::shared_ptr<CfgNode>& child) {
    children.insert(child);
  }

  void addStmtNumber(int stmtNumber) {
    statementNumberSet.insert(stmtNumber);
  }

  void setParentNode(const std::shared_ptr<CfgNode>& parent) {
    parentNode = parent;
  }

  std::shared_ptr<CfgNode> getParentNode(int stmtNumber) {
    std::shared_ptr<CfgNode> parent = parentNode;
    while (parent != nullptr) {
      if (parent->statementNumberSet.size() == 1 && *parent->statementNumberSet.begin() == stmtNumber) {
        return parent;
      } else {
        parent = parent->parentNode;
      }
    }
    return parentNode;
  }

  bool isEndNode() {
    if (statementNumberSet.size() == 1) {
      return *statementNumberSet.begin() == -1;
    }
    return false;
  }
};
