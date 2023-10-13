#include "Cfg.h"

std::shared_ptr<CfgNode> Cfg::rootCfgNode = std::make_shared<CfgNode>(0);

std::shared_ptr<CfgNode> Cfg::currNode = rootCfgNode;

std::stack<std::shared_ptr<CfgNode>> Cfg::elseEndNodeStack = std::stack<std::shared_ptr<CfgNode>>();

std::stack<std::shared_ptr<CfgNode>> Cfg::keyNodesStack = std::stack<std::shared_ptr<CfgNode>>();

void Cfg::handleStatement(int stmtNumber) {
  currNode->addStmtNumber(stmtNumber);
}

void Cfg::handleIfStatement(int stmtNumber) {
  std::shared_ptr<CfgNode> ifNode = std::make_shared<CfgNode>(stmtNumber);
  keyNodesStack.push(ifNode);
  currNode->addChildren(ifNode);
  currNode = ifNode;
}

void Cfg::handleElseStatement() {
  std::shared_ptr<CfgNode> elseNode = std::make_shared<CfgNode>();
  std::shared_ptr<CfgNode> parent = keyNodesStack.top();
  keyNodesStack.pop();
  parent->addChildren(elseNode);
  keyNodesStack.push(currNode);
  currNode = elseNode;
}

void Cfg::handleWhileStatement(int stmtNumber) {
  std::shared_ptr<CfgNode> whileNode = std::make_shared<CfgNode>(stmtNumber);
  currNode->addChildren(whileNode);
  keyNodesStack.push(whileNode);
  std::shared_ptr<CfgNode> nextNode = std::make_shared<CfgNode>();
  whileNode->addChildren(nextNode);
  currNode = nextNode;
}

void Cfg::handleEndProcedureStatement() {
}

void Cfg::handleEndElseStatement() {
    std::shared_ptr<CfgNode> endNode = keyNodesStack.top();
    keyNodesStack.pop();
    currNode->addChildren(endNode);
    currNode = endNode;
}

void Cfg::handleEndWhileStatement() {
  std::shared_ptr<CfgNode> parentNode = keyNodesStack.top();
  keyNodesStack.pop();
  currNode->addChildren(parentNode);
  std::shared_ptr<CfgNode> nextNode = std::make_shared<CfgNode>();
  currNode->addChildren(nextNode);
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

