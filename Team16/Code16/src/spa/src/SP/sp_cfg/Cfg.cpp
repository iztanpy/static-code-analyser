#include "Cfg.h"

CfgNodePtr CfgStatementHandler::handleStatement(CfgNodePtr cfgNode, int stmtNumber) {
  cfgNode->addStmtNumber(stmtNumber);
  return cfgNode;
}

CfgNodePtr IfCfgStatementHandler::handleStatement(CfgNodePtr cfgNode, int stmtNumber) {
  CfgNodePtr ifNode = std::make_shared<CfgNode>(stmtNumber);
  cfgNode->addChildren(ifNode);
  ifNode->setParentNode(cfgNode);
  return ifNode;
}

CfgNodePtr ElseCfgStatementHandler::handleStatement(CfgNodePtr cfgNode, int stmtNumber) {
  CfgNodePtr elseNode = std::make_shared<CfgNode>(stmtNumber);
  cfgNode->addChildren(elseNode);
//  elseNode->addParent(cfgNode); keeping if parent for now not sure if pkb needs this
  return elseNode;
}

CfgNodePtr WhileCfgStatementHandler::handleStatement(CfgNodePtr cfgNode, int stmtNumber) {
  CfgNodePtr whileNode = std::make_shared<CfgNode>(stmtNumber);
  cfgNode->addChildren(whileNode);
  return whileNode;
}

CfgNodePtr CloseBraceCfgStatementHandler::addEndCfgNode(CfgNodePtr cfgNode) {
  CfgNodePtr endNode = std::make_shared<CfgNode>(-1);
  cfgNode->addChildren(endNode);
  return endNode;
}

CfgNodePtr CloseBraceCfgStatementHandler::handleStatement(CfgNodePtr cfgNode, int stmtNumber) {
  // traverse to parent if or while node
  CfgNodePtr parent = cfgNode->getParentNode(); //make sure to only get the if or while and not else
  // if its an end to a if/else create a dummy end node
  CfgNodePtr currNode = addEndCfgNode(cfgNode);

  currNode->addChildren(parent);
  return currNode;
}

CfgNodeMap Cfg::cfgNodeMap = CfgNodeMap();

std::shared_ptr<CfgStatementHandler> Cfg::cfgStatementHandler = std::make_shared<CfgStatementHandler>();

CfgNodeMap Cfg::getCfgNodes() {
  return cfgNodeMap;
}

void Cfg::addCfgNodeToMap(const string& procedureName, CfgNodePtr cfgNode) {
  cfgNodeMap[procedureName] = cfgNode;
}

CfgNodePtr Cfg::handleStatement(CfgNodePtr cfgNode, int stmtNumber) {
    return cfgStatementHandler->handleStatement(cfgNode, stmtNumber);
}