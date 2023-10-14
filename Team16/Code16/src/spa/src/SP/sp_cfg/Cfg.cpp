#include "Cfg.h"

std::shared_ptr<CfgNode> Cfg::rootCfgNode = std::make_shared<CfgNode>();

std::shared_ptr<CfgNode> Cfg::currNode = rootCfgNode;

std::stack<std::shared_ptr<CfgNode>> Cfg::keyNodesStack = std::stack<std::shared_ptr<CfgNode>>();

void Cfg::handleStatement(int stmtNumber) {
  currNode->addStmtNumber(stmtNumber);
}

void Cfg::handleIfStatement(int stmtNumber) {
  if (currNode->getStmtNumberSet().empty()) {
    currNode->addStmtNumber(stmtNumber);
  } else {
    std::shared_ptr<CfgNode> ifNode = std::make_shared<CfgNode>(stmtNumber);
    currNode->addChildren(ifNode);
    currNode = ifNode;
  }
  keyNodesStack.push(currNode);
  std::shared_ptr<CfgNode> nextNode = std::make_shared<CfgNode>();
  currNode->addChildren(nextNode);
  currNode = nextNode;
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
  if (currNode->getStmtNumberSet().empty()) {
    currNode->addStmtNumber(stmtNumber);
  } else {
    std::shared_ptr<CfgNode> whileNode = std::make_shared<CfgNode>(stmtNumber);
    currNode->addChildren(whileNode);
    currNode = whileNode;
  }
  keyNodesStack.push(currNode);
  std::shared_ptr<CfgNode> nextNode = std::make_shared<CfgNode>();
  currNode->addChildren(nextNode);
  currNode = nextNode;
}

void Cfg::handleEndProcedureStatement() {
  rootCfgNode = std::make_shared<CfgNode>();
  currNode = rootCfgNode;
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

