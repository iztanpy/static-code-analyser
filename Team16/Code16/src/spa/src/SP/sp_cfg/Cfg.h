#pragma once

#include <string>
#include <unordered_map>

#include "CfgNode.h"
class Cfg {
private:
    static std::shared_ptr<CfgNode> rootCfgNode;
    static std::shared_ptr<CfgNode> currNode;

public:
    Cfg();
    ~Cfg(); // Remove the 'virtual' keyword

    static std::shared_ptr<CfgNode> getCfgNode();
    static void handleStatement(int stmtNumber);
    static void handleIfStatement(int stmtNumber);
    static void handleElseStatement(int stmtNumber);
    static void handleWhileStatement(int stmtNumber);
    static void handleEndStatement();
};

