#include "SP/sp_cfg/cfg.h"

std::shared_ptr<cfg_node> cfg::rootCfgNode = std::make_shared<cfg_node>();

std::shared_ptr<cfg_node> cfg::currNode = rootCfgNode;

std::stack<std::shared_ptr<cfg_node>> cfg::keyNodesStack = std::stack<std::shared_ptr<cfg_node>>();

std::stack<std::set<int>> cfg::nextParentStack = std::stack<std::set<int>>();

std::unordered_map<int, std::unordered_set<int>> cfg::nextStatementNumberHashmap
    = std::unordered_map<int, std::unordered_set<int>>();

std::unordered_map<int, std::shared_ptr<cfg_node>> cfg::stmtNumberToCfgNodeHashmap
    = std::unordered_map<int, std::shared_ptr<cfg_node>>();

std::unordered_map<std::string, std::shared_ptr<cfg_node>> cfg::cfgNodeMap
    = std::unordered_map<std::string, std::shared_ptr<cfg_node>>();

std::unordered_map<int, std::shared_ptr<cfg_node>> cfg::getStmtNumberToCfgNodeHashmap() {
  return stmtNumberToCfgNodeHashmap;
}

void cfg::addCfgNodeToMap(int stmtNumber) {
  stmtNumberToCfgNodeHashmap[stmtNumber] = currNode;
}

void cfg::handleStatement(int stmtNumber) {
  storeNextRelationship(currNode->getLastStatementNumber(), stmtNumber, false);
  currNode->addStmtNumber(stmtNumber);
  addCfgNodeToMap(stmtNumber);
}

void cfg::handleIfStatement(int stmtNumber) {
  storeNextRelationship(currNode->getLastStatementNumber(), stmtNumber);
  addStmtNumberToEmptyOrNewCfgNode(stmtNumber);
  createNewEmptyCfgNode();
}

void cfg::handleElseStatement() {
  std::shared_ptr<cfg_node> parent = retrieveTopKeyNode();
  nextParentStack.push({ parent->getLastStatementNumber() });
  keyNodesStack.push(currNode);
  currNode = parent;
  createNewEmptyCfgNode();
}

void cfg::handleWhileStatement(int stmtNumber) {
  storeNextRelationship(currNode->getLastStatementNumber(), stmtNumber);
  addStmtNumberToEmptyOrNewCfgNode(stmtNumber);
  createNewEmptyCfgNode();
}

void cfg::handleEndProcedureStatement() {
  while (!nextParentStack.empty()) {
    nextParentStack.pop();
  }
  rootCfgNode = std::make_shared<cfg_node>();
  currNode = rootCfgNode;
}

void cfg::handleEndElseStatement() {
  std::shared_ptr<cfg_node> endNode = retrieveTopKeyNode();
  currNode->addChildren(endNode);
  pushLastIfElseNumbersOntoNextStack();
  currNode = endNode;
}

void cfg::handleEndWhileStatement() {
  std::shared_ptr<cfg_node> parentNode = retrieveTopKeyNode();
  currNode->addChildren(parentNode);
  storeNextRelationship(currNode->getLastStatementNumber(), parentNode->getLastStatementNumber());
  currNode = parentNode;
  createNewEmptyCfgNode();
}

void cfg::handleEndIfStatement(bool hasElse) {
  if (!currNode->getStmtNumberSet().empty()) {
    int lastIfNumber = currNode->getLastStatementNumber();
    nextParentStack.push({ lastIfNumber });
  }
  createNewEmptyCfgNode();
  if (!hasElse) {
    keyNodesStack.pop();
  }
}

std::shared_ptr<cfg_node> cfg::getRootCfgNode() {
  return rootCfgNode;
}

std::unordered_map<int, std::unordered_set<int>> cfg::getNextStatementNumberHashmap() {
  return nextStatementNumberHashmap;
}

std::unordered_map<std::string, std::shared_ptr<cfg_node>> cfg::getCfgNodeHashmap() {
  return cfgNodeMap;
}

std::shared_ptr<cfg_node> cfg::retrieveTopKeyNode() {
  std::shared_ptr<cfg_node> topKeyNode = keyNodesStack.top();
  keyNodesStack.pop();
  return topKeyNode;
}

void cfg::createNewEmptyCfgNode() {
  std::shared_ptr<cfg_node> nextNode = std::make_shared<cfg_node>();
  currNode->addChildren(nextNode);
  currNode = nextNode;
}

void cfg::addStmtNumberToEmptyOrNewCfgNode(int stmtNumber) {
  if (currNode->getStmtNumberSet().empty()) {
    currNode->addStmtNumber(stmtNumber);
  } else {
    std::shared_ptr<cfg_node> newNode = std::make_shared<cfg_node>(stmtNumber);
    currNode->addChildren(newNode);
    currNode = newNode;
  }
  addCfgNodeToMap(stmtNumber);
  keyNodesStack.push(currNode);
}

void cfg::retrieveParentIfNotEmpty(int stmtNumber) {
  if (!nextParentStack.empty()) {
    std::set parents = nextParentStack.top();
    std::set<int>::iterator it;
    for (it = parents.begin(); it != parents.end(); ++it) {
      int parent = *it;
      nextStatementNumberHashmap[parent].insert(stmtNumber);
    }
    nextParentStack.pop();
  }
}

void cfg::pushLastIfElseNumbersOntoNextStack() {
  if (currNode->getStmtNumberSet().empty()) {
    std::set<int> lastNumbersInElse = nextParentStack.top();
    nextParentStack.pop();
    std::set<int> lastNumbersInIf = nextParentStack.top();
    nextParentStack.pop();
    lastNumbersInIf.insert(lastNumbersInElse.begin(), lastNumbersInElse.end());
    nextParentStack.push(lastNumbersInIf);
  } else {
    int lastNumberInElse = currNode->getLastStatementNumber();
    std::set<int> lastNumbersInIf = nextParentStack.top();
    nextParentStack.pop();
    lastNumbersInIf.insert(lastNumberInElse);
    nextParentStack.push(lastNumbersInIf);
  }
}

void cfg::storeNextRelationship(int lastStmtNumber, int nextStmtNumber, bool shouldPushToNextStack) {
  if (lastStmtNumber == -2) {
    retrieveParentIfNotEmpty(nextStmtNumber);
  } else {
    nextStatementNumberHashmap[lastStmtNumber].insert(nextStmtNumber);
  }
  if (shouldPushToNextStack) {
    nextParentStack.push({ nextStmtNumber });
  }
}

void cfg::addToCfgNodeMap(const std::string& procedureName) {
  cfgNodeMap[procedureName] = std::move(rootCfgNode);
}
