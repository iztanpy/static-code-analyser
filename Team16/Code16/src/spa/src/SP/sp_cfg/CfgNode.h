#pragma once

#include <memory>
#include <set>
#include <unordered_set>

/**
 * @class CfgNode
 * @brief A node in a control flow graph (CFG).
 *
 * The `CfgNode` class represents a node in a control flow graph (CFG) for a program. Each node can have child nodes,
 * a parent node, and a set of associated statement numbers.
 */
class CfgNode {
 private:
  /**
   * @brief Set of child nodes connected to this node in the CFG.
  */
  std::unordered_set<std::shared_ptr<CfgNode>> children;
  std::shared_ptr<CfgNode> parentNode = nullptr;
  std::set<int> statementNumberSet;

 public:
  CfgNode() {}

  explicit CfgNode(int statementNo) {
    statementNumberSet.insert(statementNo);
  }

  virtual ~CfgNode() = default;

  /**
   * @brief Add a child node to this node in the CFG.
   *
   * @param child A shared pointer to the child node to be added.
   */
  void addChildren(const std::shared_ptr<CfgNode>& child) {
    children.insert(child);
  }

  /**
   * @brief Add a statement number to the set of statement numbers associated with this node.
   *
   * @param stmtNumber The statement number to be added.
  */
  void addStmtNumber(int stmtNumber) {
    statementNumberSet.insert(stmtNumber);
  }

  /**
   * @brief Set the parent node of this node in the CFG.
   *
   * @param parent A shared pointer to the parent node.
  */
  void setParentNode(const std::shared_ptr<CfgNode>& parent) {
    parentNode = parent;
  }

  /**
  * @brief Get the parent node associated with a specific statement number.
  *
  * This method traverses the parent nodes to find the parent associated with a specific statement number.
  *
  * @param stmtNumber The statement number for which to find the parent node.
  * @return A shared pointer to the parent node, or nullptr if not found.
  */
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

  /**
  * @brief Check if this node is an end node in the CFG.
  *
  * An end node is typically associated with a statement number of -1.
  *
  * @return `true` if this node is an end node, `false` otherwise.
  */
  bool isEndNode() {
    if (statementNumberSet.size() == 1) {
      return *statementNumberSet.begin() == -1;
    }
    return false;
  }
};
