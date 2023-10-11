#pragma once

#include <string>
#include <unordered_map>
#include "CfgNode.h"

class CfgNode;

typedef std::string string;
typedef std::unordered_map<string, CfgNode*> CfgNodeMap;
//typedef std::

class CfgStatementHandler {
 public:
  static CfgNode* handleStatement(CfgNode* cfgNode, int stmtNumber);
};

class IfCfgStatementHandler : public CfgStatementHandler {
 public:
  static CfgNode* handleStatement(CfgNode* cfgNode, int stmtNumber);
};

class ElseCfgStatementHandler : public CfgStatementHandler {
 public:
  static CfgNode* handleStatement(CfgNode* cfgNode, int stmtNumber);
};

class WhileCfgStatementHandler : public CfgStatementHandler {
 public:
  static CfgNode* handleStatement(CfgNode* cfgNode, int stmtNumber);
};

class CloseBraceCfgStatementHandler : public CfgStatementHandler {
 private:
  static void addEndCfgNode(CfgNode* cfgNode);
 public:
  static CfgNode* handleStatement(CfgNode* cfgNode, int stmtNumber);
};

class Cfg {
 private:
  static CfgNodeMap cfgNodeMap;
  static CfgStatementHandler* cfgStatementHandler;

 public:
  explicit Cfg() {
  }
  virtual ~Cfg() = default;
  static CfgNodeMap getCfgNodes();
  static void addCfgNodeToMap(const string& procedureName, CfgNode* cfgNode);
  static CfgNode* handleStatement(CfgNode* cfgNode, int stmtNumber);
};
