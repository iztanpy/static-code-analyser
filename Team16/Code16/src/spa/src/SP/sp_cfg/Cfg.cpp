#include "Cfg.h"

std::shared_ptr<CfgNode> Cfg::rootCfgNode = std::make_shared<CfgNode>(0);

std::shared_ptr<CfgNode> Cfg::currNode = rootCfgNode;

void Cfg::handleStatement(int stmtNumber) {
  currNode->addStmtNumber(stmtNumber);
}

void Cfg::handleIfStatement(int stmtNumber) {
  std::shared_ptr<CfgNode> ifNode = std::make_shared<CfgNode>(stmtNumber);
  currNode->addChildren(ifNode);
  ifNode->setParentNode(currNode);
}

void Cfg::handleElseStatement(int stmtNumber) {
  std::shared_ptr<CfgNode>  elseNode = std::make_shared<CfgNode>(stmtNumber);
  currNode->addChildren(elseNode);
//  elseNode->addParent(cfgNode); keeping if parent for now not sure if pkb needs this
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

std::shared_ptr<CfgNode> Cfg::getCfgNode() {
  return rootCfgNode;
}