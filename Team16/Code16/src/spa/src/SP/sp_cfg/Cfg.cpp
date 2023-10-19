#include "Cfg.h"

std::shared_ptr<CfgNode> Cfg::rootCfgNode = std::make_shared<CfgNode>();

std::shared_ptr<CfgNode> Cfg::currNode = rootCfgNode;

std::stack<std::shared_ptr<CfgNode>> Cfg::keyNodesStack = std::stack<std::shared_ptr<CfgNode>>();

std::stack<std::set<int>> Cfg::nextParentStack = std::stack<std::set<int>>();

std::unordered_map<int, std::set<int>> Cfg::nextStatementNumberHashmap = std::unordered_map<int, std::set<int>>();

std::unordered_map<int, std::shared_ptr<CfgNode>> Cfg::stmtNumberToCfgNodeHashmap
        = std::unordered_map<int, std::shared_ptr<CfgNode>>();

std::unordered_map<int, std::shared_ptr<CfgNode>> Cfg::getStmtNumberToCfgNodeHashmap() {
    return stmtNumberToCfgNodeHashmap;
}

void Cfg::addCfgNodeToMap(int stmtNumber) {
    stmtNumberToCfgNodeHashmap[stmtNumber] = currNode;
}

void Cfg::handleStatement(int stmtNumber) {
    storeNextRelationship(currNode->getLastStatementNumber(), stmtNumber, false);
    currNode->addStmtNumber(stmtNumber);
    addCfgNodeToMap(stmtNumber);
}

void Cfg::handleIfStatement(int stmtNumber) {
    storeNextRelationship(currNode->getLastStatementNumber(), stmtNumber);
    addStmtNumberToEmptyOrNewCfgNode(stmtNumber);
    createNewEmptyCfgNode();
}

void Cfg::handleElseStatement() {
    std::shared_ptr<CfgNode> parent = retrieveTopKeyNode();
    nextParentStack.push({ parent->getLastStatementNumber() });
    keyNodesStack.push(currNode);
    currNode = parent;
    createNewEmptyCfgNode();
}

void Cfg::handleWhileStatement(int stmtNumber) {
    storeNextRelationship(currNode->getLastStatementNumber(), stmtNumber);
    addStmtNumberToEmptyOrNewCfgNode(stmtNumber);
    createNewEmptyCfgNode();
}

void Cfg::handleEndProcedureStatement() {
    rootCfgNode = std::make_shared<CfgNode>();
    currNode = rootCfgNode;
}

void Cfg::handleEndElseStatement() {
    std::shared_ptr<CfgNode> endNode = retrieveTopKeyNode();
    currNode->addChildren(endNode);
    pushLastIfElseNumbersOntoNextStack();
    currNode = endNode;
}

void Cfg::handleEndWhileStatement() {
    std::shared_ptr<CfgNode> parentNode = retrieveTopKeyNode();
    currNode->addChildren(parentNode);
    storeNextRelationship(currNode->getLastStatementNumber(), parentNode->getLastStatementNumber());
    currNode = parentNode;
    createNewEmptyCfgNode();
}

void Cfg::handleEndIfStatement(bool hasElse) {
    if (!currNode->getStmtNumberSet().empty()) {
        int lastIfNumber = currNode->getLastStatementNumber();
        nextParentStack.push({ lastIfNumber });
    }
    createNewEmptyCfgNode();
    if (!hasElse) {
        keyNodesStack.pop();
    }
}

std::shared_ptr<CfgNode> Cfg::getCfgNode() {
    return rootCfgNode;
}

std::shared_ptr<CfgNode> Cfg::retrieveTopKeyNode() {
    std::shared_ptr<CfgNode> topKeyNode = keyNodesStack.top();
    keyNodesStack.pop();
    return topKeyNode;
}

void Cfg::createNewEmptyCfgNode() {
    std::shared_ptr<CfgNode> nextNode = std::make_shared<CfgNode>();
    currNode->addChildren(nextNode);
    currNode = nextNode;
}

void Cfg::addStmtNumberToEmptyOrNewCfgNode(int stmtNumber) {
    if (currNode->getStmtNumberSet().empty()) {
        currNode->addStmtNumber(stmtNumber);
    } else {
        std::shared_ptr<CfgNode> newNode = std::make_shared<CfgNode>(stmtNumber);
        currNode->addChildren(newNode);
        currNode = newNode;
    }
    addCfgNodeToMap(stmtNumber);
    keyNodesStack.push(currNode);
}

void Cfg::retrieveParentIfNotEmpty(int stmtNumber) {
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

void Cfg::pushLastIfElseNumbersOntoNextStack() {
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

void Cfg::storeNextRelationship(int lastStmtNumber, int nextStmtNumber, bool shouldPushToNextStack) {
    if (lastStmtNumber == -2) {
        retrieveParentIfNotEmpty(nextStmtNumber);
    } else {
        nextStatementNumberHashmap[lastStmtNumber].insert(nextStmtNumber);
    }
    if (shouldPushToNextStack) {
        nextParentStack.push({ nextStmtNumber });
    }
}
