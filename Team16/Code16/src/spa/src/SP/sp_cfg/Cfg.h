#pragma once

#include <string>
#include <memory>
#include <stack>
#include <unordered_map>

#include "CfgNode.h"
class Cfg {
private:
  static std::shared_ptr<CfgNode> currNode;

public:
  Cfg() = default;
  virtual ~Cfg() = default;

    static std::shared_ptr<CfgNode> getCfgNode();
    static void handleStatement(int stmtNumber);
    static void handleIfStatement(int stmtNumber);
    static void handleElseStatement(int stmtNumber);
    static void handleWhileStatement(int stmtNumber);
    static void handleEndStatement();
    static void handleEndElseStatement();
    static std::shared_ptr<CfgNode> rootCfgNode;
    static std::stack<std::shared_ptr<CfgNode>> elseEndNodeStack;
};

