#pragma once

#include <string>
#include <memory>
#include <stack>
#include <unordered_map>
#include <iostream>
#include <set>
#include <unordered_set>

#include "SP/sp_cfg/cfg_node.h"

/**
 * @class cfg
 * @brief A control flow graph (CFG) representation for a program.
 *
 * The `cfg` class provides functionality to build and manage a control flow graph (CFG) for a program.
 * It includes methods to handle statements and control flow constructs, as well as access to the CFG nodes.
 */
class cfg {
 private:
  /**
   * @brief Pointer to the current CFG node being constructed.
   *
   * This pointer keeps track of the current node while constructing the CFG.
   */
  static std::shared_ptr<cfg_node> currNode;
  static std::stack<std::shared_ptr<cfg_node>> keyNodesStack;
  static std::stack<std::set<int>> nextParentStack;
  static std::unordered_map<int, std::shared_ptr<cfg_node>> stmtNumberToCfgNodeHashmap;
  static std::shared_ptr<cfg_node> rootCfgNode;
  static std::unordered_map<int, std::unordered_set<int>> nextStatementNumberHashmap;
  static std::unordered_map<std::string, std::shared_ptr<cfg_node>> cfgNodeMap;
  static std::shared_ptr<cfg_node> retrieveTopKeyNode();
  static void addCfgNodeToMap(int stmtNumber);
  static void createNewEmptyCfgNode();
  static void addStmtNumberToEmptyOrNewCfgNode(int stmtNumber);
  static void pushLastIfElseNumbersOntoNextStack();
  static void storeNextRelationship(int lastStmtNumber, int nextStmtNumber, bool shouldPushToNextStack = true);

 public:
  cfg() = default;
  virtual ~cfg() = default;
  /**
  * @brief Get the current CFG node.
  *
  * This method returns a pointer to the current CFG node being constructed.
  *
  * @return A shared pointer to the current CFG node.
  */
  static std::shared_ptr<cfg_node> getCfgNode();
  /**
  * @brief Handle a general statement in the program.
  *
  * This method is used to handle a general statement in the program and update the CFG accordingly.
  *
  * @param stmtNumber The statement number of the current statement.
  */
  static void handleStatement(int stmtNumber);
  /**
  * @brief Handle an "if" statement in the program.
  *
  * This method is used to handle an "if" statement in the program and update the CFG accordingly.
  *
  * @param stmtNumber The statement number of the "if" statement.
  */
  static void handleIfStatement(int stmtNumber);
  /**
  * @brief Handle an "else" statement in the program.
  *
  * This method is used to handle an "else" statement in the program and update the CFG accordingly.
  */
  static void handleElseStatement();
  /**
  * @brief Handle a "while" statement in the program.
  *
  * This method is used to handle a "while" statement in the program and update the CFG accordingly.
  *
  * @param stmtNumber The statement number of the "while" statement.
  */
  static void handleWhileStatement(int stmtNumber);
  /**
   * @brief Handle an "end" statement.
   *
   * This method is used to handle an "end" statement in the program and update the CFG accordingly.
   */
  static void handleEndProcedureStatement();
  /**
   * @brief Handle an end of an "else" statement.
   *
   * This method is used to handle an end of an "else" statement in the program and update the CFG accordingly.
   */
  static void handleEndElseStatement();
  /**
   * @brief Handle an end of a "while" statement in the program.
   *
   * This method is used to handle an end of a "while' statement in the program and update the CFG accordingly.
   */
  static void handleEndWhileStatement();
  /**
   * @brief Handle an end of an "if" statement in the program.
   *
   * This method is used to handle an end of an "if" statement in the program and update the CFG accordingly.
  */
  static void handleEndIfStatement(bool hasElse);
  static std::unordered_map<int, std::shared_ptr<cfg_node>> getStmtNumberToCfgNodeHashmap();
  /**
   * @brief Get the current CFG node.
   *
   * This method returns a pointer to the current CFG node being constructed.
   *
   * @return A shared pointer to the current CFG node.
   */
  static std::shared_ptr<cfg_node> getRootCfgNode();
  /**
   * @brief Get the map of next statement numbers.
   *
   * This method returns a map of statement numbers to a set of next statement numbers.
   *
   * @return A map of statement numbers to a set of next statement numbers.
   */
  static std::unordered_map<int, std::unordered_set<int>> getNextStatementNumberHashmap();
  /**
   * @brief Get the map of statement number to cfg nodes.
   *
   * This method returns a map of statement numbers to its corresponding cfg nodes.
   *
   * @return A map of statement numbers to its corresponding cfg nodes.
   */
  static std::unordered_map<int, std::shared_ptr<cfg_node>> getStmtNumberToCfgNodeHashmap();
  /**
   * @brief Get the map of procedure names to cfg root nodes.
   *
   * This method returns a map of procedure names to its corresponding cfg root nodes.
   *
   * @return A map of procedure names to its corresponding cfg root nodes.
   */
  static std::unordered_map<std::string, std::shared_ptr<cfg_node>> getCfgNodeHashmap();
  /**
   * @brief Connects parent and child cfg nodes.
   *
   * This method connects parents of current cfg node to itself.
   *
   */
  static void retrieveParentIfNotEmpty(int stmtNumber);
  /**
   * @brief Resets internal maps.
   *
   * This method clears internal maps.
   *
   */
  static void resetCFG() {
    for (auto& entry : nextStatementNumberHashmap) {
        entry.second.clear();
    }
    nextStatementNumberHashmap.clear();
    stmtNumberToCfgNodeHashmap.clear();
    cfgNodeMap.clear();
    keyNodesStack = std::stack<std::shared_ptr<cfg_node>>();
    nextParentStack = std::stack<std::set<int>>();
  }
  /**
  * @brief Adds the a cfg node with its associated procedure name to internal map.
  *
  * This method is used to store the current root cfg node with its associated procedure name into an internal map.
  *
  * @param procedureName The associated procedure name.
  */
  static void addToCfgNodeMap(const std::string& procedureName);
};

