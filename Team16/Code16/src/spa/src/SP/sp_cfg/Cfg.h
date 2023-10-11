#pragma once

#include <string>
#include <unordered_map>

// Headers from "SP" subdirectory
#include "CfgNode.h"

typedef std::string string;
typedef std::shared_ptr<CfgNode> CfgNodePtr;
typedef std::unordered_map<string, CfgNodePtr> CfgNodeMap;

class CfgStatementHandler {
 public:
  static CfgNodePtr handleStatement(CfgNodePtr cfgNode, int stmtNumber);
};

class IfCfgStatementHandler : public CfgStatementHandler {
 public:
  static CfgNodePtr handleStatement(CfgNodePtr cfgNode, int stmtNumber);
};

class ElseCfgStatementHandler : public CfgStatementHandler {
 public:
  static CfgNodePtr handleStatement(CfgNodePtr cfgNode, int stmtNumber);
};

class WhileCfgStatementHandler : public CfgStatementHandler {
 public:
  static CfgNodePtr handleStatement(CfgNodePtr cfgNode, int stmtNumber);
};

class CloseBraceCfgStatementHandler : public CfgStatementHandler {
 private:
  static CfgNodePtr addEndCfgNode(CfgNodePtr cfgNode);

 public:
  static CfgNodePtr handleStatement(CfgNodePtr cfgNode, int stmtNumber);
};

class Cfg {
 private:
  static CfgNodeMap cfgNodeMap;
  static std::shared_ptr<CfgStatementHandler> cfgStatementHandler;

 public:
  Cfg() = default;
  virtual ~Cfg() = default;
  static CfgNodeMap getCfgNodes();
  static void addCfgNodeToMap(const string& procedureName, CfgNodePtr cfgNode);
  static CfgNodePtr handleStatement(CfgNodePtr cfgNode, int stmtNumber);
};
