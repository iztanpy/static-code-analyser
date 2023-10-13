#pragma once

#include <string>
#include <memory>
#include <stack>
#include <unordered_map>

#include "CfgNode.h"
/**
 * @class Cfg
 * @brief A control flow graph (CFG) representation for a program.
 *
 * The `Cfg` class provides functionality to build and manage a control flow graph (CFG) for a program.
 * It includes methods to handle statements and control flow constructs, as well as access to the CFG nodes.
 */
class Cfg {
 private:
  /**
   * @brief Pointer to the current CFG node being constructed.
   *
   * This pointer keeps track of the current node while constructing the CFG.
   */
    static std::shared_ptr<CfgNode> currNode;

 public:
    Cfg() = default;
    virtual ~Cfg() = default;
    /**
    * @brief Get the current CFG node.
    *
    * This method returns a pointer to the current CFG node being constructed.
    *
    * @return A shared pointer to the current CFG node.
    */
    static std::shared_ptr<CfgNode> getCfgNode();
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
    *
    * @param stmtNumber The statement number of the "else" statement.
    */
    static void handleElseStatement(int stmtNumber);
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
    static void handleEndStatement();
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
     *
     * @param stmtNumber The statement number of the "while" statement.
     */
    static void handleEndWhileStatement(int stmtNumber);
    /**
     * @brief Handle an end of an "if" statement in the program.
     *
     * This method is used to handle an end of an "if" statement in the program and update the CFG accordingly.
    */
    static void handleEndIfStatement();
    static std::shared_ptr<CfgNode> rootCfgNode;
    static std::stack<std::shared_ptr<CfgNode>> elseEndNodeStack;
};

