#pragma once

#include <string>
#include <unordered_map>

// Headers from "SP" subdirectory
#include "CfgNode.h"

typedef std::string string;
typedef std::shared_ptr<CfgNode> CfgNodePtr;
typedef std::unordered_map<string, CfgNodePtr> CfgNodeMap;

class Cfg {
 private:
  static std::shared_ptr<CfgNode> rootCfgNode;
  static std::shared_ptr<CfgNode> currNode;
  static CfgNodeMap cfgNodeMap;

 public:
  Cfg() {
	 currNode = rootCfgNode;
  }  
  virtual ~Cfg() = default;
  static std::shared_ptr<CfgNode> getCfgNode();
  /*static void addCfgNodeToMap(const string& procedureName, CfgNodePtr cfgNode);*/
  static void handleStatement(int stmtNumber);
  static void handleIfStatement(int stmtNumber);
  static void handleElseStatement(int stmtNumber);
  static void handleWhileStatement(int stmtNumber);
  static void handleEndStatement();
};
