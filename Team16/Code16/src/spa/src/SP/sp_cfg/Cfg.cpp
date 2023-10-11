#include "Cfg.h"


CfgNode* CfgStatementHandler::handleStatement(CfgNode* cfgNode, int stmtNumber) {
  cfgNode->addStatement(stmtNumber);
  return cfgNode;
}

CfgNode* IfCfgStatementHandler::handleStatement(CfgNode* cfgNode, int stmtNumber) {
  CfgNode ifNode = new CfgNode(stmtNumber);
  cfgNode->addChild(ifNode);
  ifNode->addParent(cfgNode);
  return ifNode;
}

CfgNode* ElseCfgStatementHandler::handleStatement(CfgNode* cfgNode, int stmtNumber) {
  CfgNode elseNode = new CfgNode(stmtNumber);
  cfgNode->addChild(elseNode);
//  elseNode->addParent(cfgNode); keeping if parent for now not sure if pkb needs this
  return elseNode;
}

CfgNode* WhileCfgStatementHandler::handleStatement(CfgNode* cfgNode, int stmtNumber) {
  CfgNode whileNode = new CfgNode(stmtNumber);
  cfgNode->addChild(whileNode);
  return whileNode;
}

CfgNode* CloseBraceCfgStatementHandler::addEndCfgNode(CfgNode* cfgNode) {
  CfgNode end = new CfgNode("end");
  cfgNode->addChild(end);
  return cfgNode;
}

CfgNode* CloseBraceCfgStatementHandler::handleStatement(CfgNode* cfgNode, int stmtNumber) {
  // traverse to parent if or while node
  CfgNode* parent = cfgNode->getParent(); //make sure to only get the if or while and not else
  while (parent != nullptr) {
    if parent->getType() == "if" || parent->getType() == "while" {
      if parent.hasEnded() != true { // already has an end node pointing to it
        break;
      }
    }
  }
  // if its an end to a if/else create a dummy end node
  CfgNode* currNode = addEndCfgNode(cfgNode);

  currNode->addChild(parent);
  return currNode;
}

CfgNodeMap Cfg::cfgNodeMap = CfgNodeMap();

CfgStatementHandler* Cfg::cfgStatementHandler = new CfgStatementHandler();

CfgNodeMap Cfg::getCfgNodes() {
  return cfgNodeMap;
}

void Cfg::addCfgNodeToMap(const string& procedureName, CfgNode* cfgNode) {
  cfgNodeMap[procedureName] = cfgNode;
}

CfgNode* Cfg::handleStatement(CfgNode* cfgNode, int stmtNumber) {
    return cfgStatementHandler->handleStatement(cfgNode, stmtNumber);
}