#pragma once

#include <memory>
#include <set>
#include <unordered_set>

/**
 * @class cfg_node
 * @brief A node in a control flow graph (CFG).
 *
 * The `cfg_node` class represents a node in a control flow graph (CFG) for a program. Each node can have child nodes,
 * a parent node, and a set of associated statement numbers.
 */
class cfg_node {
 private:
  /**
   * @brief Set of child nodes connected to this node in the CFG.
  */
  std::set<std::shared_ptr<cfg_node>> children;
  std::shared_ptr<cfg_node> parentNode = nullptr;
  std::set<int> statementNumberSet;

 public:
  cfg_node() {}

  explicit cfg_node(int statementNo) {
    statementNumberSet.insert(statementNo);
  }

  virtual ~cfg_node() = default;

  /**
   * @brief Add a child node to this node in the CFG.
   *
   * @param child A shared pointer to the child node to be added.
   */
  void addChildren(const std::shared_ptr<cfg_node>& child) {
    children.insert(child);
  }
  /**
   * @brief Get children nodes to this node in the CFG.
   */
  std::set<std::shared_ptr<cfg_node>> getChildren() {
    return children;
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
   * @brief Get the set of statement numbers associated with this node.
  */
  std::set<int> getStmtNumberSet() {
    return statementNumberSet;
  }
  /**
  * @brief Gets the last statement number within the statement number hashset.
  *
  * If the last statement can't be found, it will return a value of -2.
  *
  * @return last statement number if exists, else return -2.
  */
  int getLastStatementNumber() {
    auto lastElementIt = statementNumberSet.rbegin();
    if (lastElementIt != statementNumberSet.rend()) {
      return *lastElementIt;
    }
    return -2;
  }
};
