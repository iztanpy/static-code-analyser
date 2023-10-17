#include "Cfg.h"

std::shared_ptr<CfgNode> Cfg::rootCfgNode = std::make_shared<CfgNode>();

std::shared_ptr<CfgNode> Cfg::currNode = rootCfgNode;

std::stack<std::shared_ptr<CfgNode>> Cfg::keyNodesStack = std::stack<std::shared_ptr<CfgNode>>();

std::stack<std::set<int>> Cfg::nextParentStack = std::stack<std::set<int>>();

std::unordered_map<int, std::set<int>> Cfg::nextStatementNumberHashmap = std::unordered_map<int, std::set<int>>();



void Cfg::handleStatement(int stmtNumber) {
    if (currNode->getStmtNumberSet().empty() && !nextParentStack.empty()) {
        retrieveParent(stmtNumber);
    }
    if (!currNode->getStmtNumberSet().empty()) {
        nextStatementNumberHashmap[currNode->getLastStatementNumber()].insert(stmtNumber); 
    }
    currNode->addStmtNumber(stmtNumber);
}

void Cfg::handleIfStatement(int stmtNumber) {
  if (currNode->getStmtNumberSet().empty()) {
    if (!nextParentStack.empty()) {
        retrieveParent(stmtNumber);
    }
    currNode->addStmtNumber(stmtNumber);
  } else {
    std::shared_ptr<CfgNode> ifNode = std::make_shared<CfgNode>(stmtNumber);
    currNode->addChildren(ifNode);
    currNode = ifNode;
  }
  keyNodesStack.push(currNode);
  nextParentStack.push({ stmtNumber });
  std::shared_ptr<CfgNode> nextNode = std::make_shared<CfgNode>();
  currNode->addChildren(nextNode);
  currNode = nextNode;
}

void Cfg::handleElseStatement() {
  std::shared_ptr<CfgNode> elseNode = std::make_shared<CfgNode>();
  std::shared_ptr<CfgNode> parent = keyNodesStack.top();
  keyNodesStack.pop();
  nextParentStack.push({ parent->getLastStatementNumber() });
  parent->addChildren(elseNode);
  keyNodesStack.push(currNode);
  currNode = elseNode;
}

void Cfg::handleWhileStatement(int stmtNumber) {
  if (currNode->getStmtNumberSet().empty()) {
    if (!nextParentStack.empty()) {
        retrieveParent(stmtNumber);
    }
    currNode->addStmtNumber(stmtNumber);
  } else {
    nextStatementNumberHashmap[currNode->getLastStatementNumber()].insert(stmtNumber);
    std::shared_ptr<CfgNode> whileNode = std::make_shared<CfgNode>(stmtNumber);
    currNode->addChildren(whileNode);
    currNode = whileNode;
  }
  keyNodesStack.push(currNode);
  nextParentStack.push({ stmtNumber });
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
    std::set<int> previousParent = nextParentStack.top(); 
    previousParent.insert(currNode->getLastStatementNumber());
    nextParentStack.pop();
    nextParentStack.push(previousParent);
    currNode = endNode;
}

void Cfg::handleEndWhileStatement() {
  // need to handle the case where curr node is empty (i.e. previous node is an end node from if else) 
  std::shared_ptr<CfgNode> parentNode = keyNodesStack.top();
  keyNodesStack.pop();
  currNode->addChildren(parentNode);
  if (currNode->getStmtNumberSet().empty()) {
      if (!nextParentStack.empty()) {
          retrieveParent(parentNode->getLastStatementNumber());
          nextParentStack.push({ parentNode->getLastStatementNumber() });
      }
  } else {
      nextStatementNumberHashmap[currNode->getLastStatementNumber()].insert(parentNode->getLastStatementNumber());
      nextParentStack.push({ parentNode->getLastStatementNumber() });
  }
  std::shared_ptr<CfgNode> nextNode = std::make_shared<CfgNode>();
  parentNode->addChildren(nextNode);
  currNode = nextNode;
}

void Cfg::handleEndIfStatement(bool hasElse) {
    std::shared_ptr<CfgNode> nextNode = std::make_shared<CfgNode>();
    currNode->addChildren(nextNode);
    nextParentStack.push({ currNode->getLastStatementNumber() });
    currNode = nextNode;
    if (hasElse == false) {
      keyNodesStack.pop();
    }
}

std::shared_ptr<CfgNode> Cfg::getCfgNode() {
  return rootCfgNode;
}

void Cfg::retrieveParent(int stmtNumber) {
    std::set parents = nextParentStack.top();
    std::set<int>::iterator it;
    for (it = parents.begin(); it != parents.end(); ++it) {
        int parent = *it;
        nextStatementNumberHashmap[parent].insert(stmtNumber);
    }
    nextParentStack.pop();
}
