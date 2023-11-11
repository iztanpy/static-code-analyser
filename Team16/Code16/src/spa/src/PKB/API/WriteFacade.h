#pragma once
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <memory>
#include <utility>
#include "PKB/PKB.h"

typedef std::string variable;
typedef int statementNumber;
typedef std::string possibleCombinations;
typedef std::string constant;

class WriteFacade {
 private:
  PKB& pkb;

 public:
  explicit WriteFacade(PKB& pkb);

  /**
  * @brief Stores assignments information in the program.
  *
  * This method stores assignment information, including mappings of statement numbers to sets of possible combinations
  * (numRHSMap) and statement numbers to variables (numLHSMap).
  *
  * @param numRHSMap An unordered map of statement numbers to sets of possible combinations for the right-hand side of assignments.
  * @param numLHSMap An unordered map of statement numbers to variables for the left-hand side of assignments.
  */
  void storeAssignments(std::unordered_map<statementNumber, std::unordered_set<possibleCombinations>> numRHSMap,
                        std::unordered_map<statementNumber, variable> numLHSMap);

  void storeAssignments(std::unordered_map<statementNumber, std::unordered_set<partialMatch>> partialRHSMap,
                        std::unordered_map<statementNumber, full> fullRHSMap,
                        std::unordered_map<statementNumber, variable> numLHSMap);

  /**
  * @brief Stores variable information in the program.
  *
  * This method stores variable information by accepting a set of variables and adding them to the program's variable store.
  *
  * @param variables An unordered set of variables to be stored.
  */
  void storeVariables(std::unordered_set<variable> variables);

  /**
  * @brief Stores procedure information in the program.
  *
  * This method stores procedure information by accepting a set of procedure names and adding them to the program's procedure store.
  *
  * @param procedures A set of procedure names to be stored.
  */
  void storeProcedures(std::set<std::string> procedures);

  void storeProcedures(std::unordered_map<procedure, std::pair<int, int>> procedures);

  /**
  * @brief Stores uses information in the program.
  *
  * This method stores uses information, including mappings of statement numbers to sets of used variables (varUsesMap).
  *
  * @param varUsesMap An unordered map of statement numbers to sets of variables that are used.
  */
  void storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap);

  /**
  * @brief Stores modifies information in the program.
  *
  * This method stores modifies information, including mappings of statement numbers to modified variables (varModifiesMap).
  *
  * @param varModifiesMap An unordered map of statement numbers to variables that are modified.
  */
  void storeModifies(std::unordered_map<statementNumber, variable> varModifiesMap);

  /**
  * @brief Stores constant information in the program.
  *
  * This method stores constant information by accepting a set of constants and adding them to the program's constant store.
  *
  * @param constants An unordered set of constants to be stored.
  */
  void storeConstants(std::unordered_set<constant> constants);

  /**
  * @brief Stores statement information in the program.
  *
  * This method stores statement information, including mappings of statement numbers to statement entities (typeMap).
  *
  * @param typeMap An unordered map of statement numbers to statement entities.
  */
  void storeStatements(std::unordered_map<statementNumber, StmtEntity> typeMap);

  /**
  * @brief Stores parent relationship information in the program.
  *
  * This method stores parent relationship information, including mappings of statement numbers to sets of parent statements (map).
  *
  * @param map An unordered map of statement numbers to sets of parent statement numbers.
  */
  void storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map);

  /**
  * @brief Stores follows relationship information in the program.
  *
  * This method stores follows relationship information, including mappings of statement numbers to their follower statement numbers (map).
  *
  * @param map An unordered map of statement numbers to their follower statement numbers.
  */
  void storeFollows(std::unordered_map<statementNumber, statementNumber> map);

  /**
  * @brief Stores a mapping of statement numbers to sets of variables for WHILE statements.
  *
  * This function allows you to store a mapping of statement numbers to sets of variables
  * for WHILE statements in the provided variableMap.
  *
  * @param variableMap A std::unordered_map where statementNumber is the key,
  *                    and std::unordered_set<variable> is the associated value.
  */
  void storeWhile(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap);

  /**
  * @brief Stores a mapping of statement numbers to sets of variables for IF statements.
  *
  * This function allows you to store a mapping of statement numbers to sets of variables
  * for IF statements in the provided variableMap.
  *
  * @param variableMap A std::unordered_map where statementNumber is the key,
  *                    and std::unordered_set<variable> is the associated value.
  */
  void storeIf(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap);

  /**
  * @brief Stores a call relationship table.
  *
  * This function stores a call relationship table, mapping procedures to the set of procedures they call.
  *
  * @param callTable An unordered_map where the key is a procedure, and the value is a set of
  *                  procedures called by the key procedure.
  */
  void storeCalls(std::unordered_map<procedure, std::unordered_set<procedure>> callTable);

  /**
  * @brief Stores a mapping of statement numbers to procedures for CALL statements.
  *
  * This function allows you to store a mapping of statement numbers to procedures
  * for CALL statements in the provided callStatements.
  *
  * @param callStatements A std::unordered_map where statementNumber is the key,
  *                      and procedure is the associated value.
  */
  void storeCallStatements(std::unordered_map<statementNumber, procedure> callStatements);

  void storeNext(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap);

  void storeCfg(std::unordered_map<std::string, std::shared_ptr<CfgNode>> cfgRoots);

  void storeCfgLegend(std::unordered_map<statementNumber, std::shared_ptr<CfgNode>> cfgLegend);
};
