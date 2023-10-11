#pragma once

#include <memory>
#include <set>
#include <unordered_set>

/**
 * @class CfgNode
 * @brief Represents a node in a context-free grammar (CFG) parse tree.
 *
 * The CfgNode class is used to represent a node in CFG (control flow graph). Each node may have
 * children, a parent node, and an associated statement number.
 */
class CfgNode {
private:
	std::unordered_set<std::shared_ptr<CfgNode>> children; // children don't need ordering
	std::shared_ptr<CfgNode> parentNode = nullptr;
	std::set<int> statementNumberSet;
public:
	explicit CfgNode(int statementNo) {
      statementNumberSet.insert(statementNo);
    }
    virtual ~CfgNode() = default;
	/**
	* @brief Add a child node to this CfgNode.
	* @param child A shared pointer to the child CfgNode.
	*/
	void addChildren(const std::shared_ptr<CfgNode>& child) {
	  this->children.insert(child);
	}
    /**
    * @brief Add a statement number to this CfgNode.
    * @param stmtNumber A statement number to be added to this CfgNode.
    */
    void addStmtNumber(int stmtNumber) {
      this->statementNumberSet.insert(stmtNumber);
    }
	/**
	* @brief Set parent node of this CfgNode
	* @param child A shared pointer to the parentNode
	*/
	void setParentNode(const std::shared_ptr<CfgNode>& parent) {
	  this->parentNode = parent;
	}
    /**
    * @brief Get parent node of this CfgNode
    * @return A parent of this node
    */
    std::shared_ptr<CfgNode> getParentNode() {
      return this->parentNode;
    }
    /**
    * @brief Checks if this CfgNode is an end node.
    * @return True if this CfgNode is an end node, false otherwise.
    */
    bool isEndNode() {
      if (this->statementNumberSet.size() == 1) {
        return *this->statementNumberSet.begin() == -1;
      }
      return false;
    }
}


