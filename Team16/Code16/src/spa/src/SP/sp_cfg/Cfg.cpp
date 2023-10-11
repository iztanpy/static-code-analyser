#include "Cfg.h"

void Cfg::handleStatement(int stmtNumber) {
  cfgNode->addStatement(stmtNumber);
  return cfgNode;
}

void Cfg::handleIfStatement(int stmtNumber) {
  CfgNodePtr ifNode = std::make_shared<CfgNode>(stmtNumber);
  cfgNode->addChildren(ifNode);
  ifNode->setParentNode(cfgNode);
  return ifNode;
}

void Cfg::handleElseStatement(int stmtNumber) {
  CfgNodePtr elseNode = std::make_shared<CfgNode>(stmtNumber);
  cfgNode->addChildren(elseNode);
//  elseNode->addParent(cfgNode); keeping if parent for now not sure if pkb needs this
  return elseNode;
}

void Cfg::handleWhileStatement(int stmtNumber) {
  CfgNodePtr whileNode = std::make_shared<CfgNode>(stmtNumber);
  cfgNode->addChildren(whileNode);
  return whileNode;
}

void Cfg::handleEndStatement() {
  CfgNodePtr endNode = std::make_shared<CfgNode>(-1);
  cfgNode->addChildren(endNode);
  return endNode;
}

  // if its an end to a if/else create a dummy end node
  CfgNodePtr currNode = addEndCfgNode(cfgNode);

  currNode->addChildren(parent);
  return currNode;
}

  // if its an end to a if/else create a dummy end node
  CfgNodePtr currNode = addEndCfgNode(cfgNode);

  currNode->addChildren(parent);
  return currNode;
}

  // if its an end to a if/else create a dummy end node
  CfgNodePtr currNode = addEndCfgNode(cfgNode);

  currNode->addChildren(parent);
  return currNode;
}

CfgNodeMap Cfg::cfgNodeMap = CfgNodeMap();

std::shared_ptr<CfgStatementHandler> Cfg::cfgStatementHandler = std::make_shared<CfgStatementHandler>();

std::shared_ptr<CfgNode> Cfg::getCfgNode() {
  return rootCfgNode;
}

/*void Cfg::addCfgNodeToMap(const string& procedureName, CfgNodePtr cfgNode) {
  cfgNodeMap[procedureName] = cfgNode;
}*/

CfgNodePtr Cfg::handleStatement(CfgNodePtr cfgNode, int stmtNumber) {
    return cfgStatementHandler->handleStatement(cfgNode, stmtNumber);
}