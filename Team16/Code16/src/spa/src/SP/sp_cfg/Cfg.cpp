#include "Cfg.h"

CfgNodePtr Cfg::rootCfgNode = std::make_shared<CfgNode>(0);

CfgNodePtr Cfg::currNode = rootCfgNode;

void Cfg::handleStatement(int stmtNumber) {
  currNode->addStmtNumber(stmtNumber);
}

void Cfg::handleIfStatement(int stmtNumber) {
  CfgNodePtr ifNode = std::make_shared<CfgNode>(stmtNumber);
  currNode->addChildren(ifNode);
  ifNode->setParentNode(currNode);
}

void Cfg::handleElseStatement(int stmtNumber) {
  CfgNodePtr elseNode = std::make_shared<CfgNode>(stmtNumber);
  currNode->addChildren(elseNode);
//  elseNode->addParent(cfgNode); keeping if parent for now not sure if pkb needs this
}

void Cfg::handleWhileStatement(int stmtNumber) {
  CfgNodePtr whileNode = std::make_shared<CfgNode>(stmtNumber);
  currNode->addChildren(whileNode);
}

void Cfg::handleEndStatement() {
  CfgNodePtr endNode = std::make_shared<CfgNode>(-1);
  currNode->addChildren(endNode);
}

CfgNodeMap Cfg::cfgNodeMap = CfgNodeMap();

CfgNodePtr Cfg::getCfgNode() {
  return rootCfgNode;
}