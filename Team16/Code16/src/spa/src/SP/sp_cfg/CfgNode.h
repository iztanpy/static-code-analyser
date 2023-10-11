#pragma once

#include <memory>
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
	std::shared_ptr<CfgNode> parentNode;
	int statementNumber;
public:
	explicit CfgNode(int statementNo) : statementNumber(statementNo) {}
	virtual ~CfgNode() = default;
	/**
	* @brief Add a child node to this CfgNode.
	* @param child A shared pointer to the child CfgNode.
	*/
	void addChildren(const std::shared_ptr<CfgNode>& child) {
	  this->children.insert(child);
	}
	/**
	* @brief Set parent node of this CfgNode
	* @param child A shared pointer to the parentNode
	*/
	void setParentNode(const std::shared_ptr<CfgNode>& parent) {
	  this->parentNode = parentNode;
	}
}


