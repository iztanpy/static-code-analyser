#include "Cfg.h"

std::shared_ptr<CfgNode> Cfg::rootCfgNode = std::make_shared<CfgNode>(0);

std::shared_ptr<CfgNode> Cfg::currNode = rootCfgNode;

std::stack<std::shared_ptr<CfgNode>> Cfg::elseEndNodeStack = std::stack<std::shared_ptr<CfgNode>>();

void Cfg::handleStatement(int stmtNumber) {
  currNode->addStmtNumber(stmtNumber);
}

void Cfg::handleIfStatement(int stmtNumber) {
  std::shared_ptr<CfgNode> ifNode = std::make_shared<CfgNode>(stmtNumber);
  currNode->addChildren(ifNode);
  ifNode->setParentNode(currNode);
}

void Cfg::handleElseStatement(int parentStmtNumber) {
  std::shared_ptr<CfgNode> endNode = std::make_shared<CfgNode>(-1);
  currNode->addChildren(endNode);
  elseEndNodeStack.push(endNode);
  std::shared_ptr<CfgNode> elseNode = std::make_shared<CfgNode>();
  std::shared_ptr<CfgNode> parent = currNode->getParentNode(parentStmtNumber);
  parent->addChildren(elseNode);
  currNode = elseNode;
}

void Cfg::handleWhileStatement(int stmtNumber) {
  std::shared_ptr<CfgNode> whileNode = std::make_shared<CfgNode>(stmtNumber);
  currNode->addChildren(whileNode);
  std::shared_ptr<CfgNode> nextNode = std::make_shared<CfgNode>();
  whileNode->addChildren(nextNode);
  currNode = nextNode;
}

void Cfg::handleEndStatement() {
  std::shared_ptr<CfgNode> endNode = std::make_shared<CfgNode>(-1);
  currNode->addChildren(endNode);
}

void Cfg::handleEndElseStatement() {
    std::shared_ptr<CfgNode> endNode = elseEndNodeStack.top();
    currNode->addChildren(endNode);
    currNode = endNode;
    elseEndNodeStack.pop();
}

void Cfg::handleEndWhileStatement(int stmtNumber) {
  // link the currnode to the parent node
  std::shared_ptr<CfgNode> parentNode = currNode->getParentNode(stmtNumber);
  currNode->addChildren(parentNode);
  std::shared_ptr<CfgNode> nextNode = std::make_shared<CfgNode>();
  parentNode->addChildren(nextNode);
  currNode = nextNode;
}

void Cfg::handleEndIfStatement() {
    std::shared_ptr<CfgNode> nextNode = std::make_shared<CfgNode>();
    currNode->addChildren(nextNode);
    currNode = nextNode;
}

std::shared_ptr<CfgNode> Cfg::getCfgNode() {
  return rootCfgNode;
}

