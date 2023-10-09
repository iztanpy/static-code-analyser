#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <utility>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include "Stores/VariableStore.h"
#include "Stores/AssignStore.h"
#include "Stores/UsesStore.h"
#include "Stores/ConstantStore.h"
#include "Stores/StatementStore.h"
#include "Stores/ParentStore.h"
#include "Stores/FollowsStore.h"
#include "Stores/ModifiesStore.h"
#include "Stores/ProcedureStore.h"
#include "Stores/CallStore.h"
#include "utils/entity_types.h"
#include "utils/clauses_types.h"
#include "utils/hash_utils.h"

typedef std::string variable;
typedef int statementNumber;
typedef std::string full;
typedef std::string constant;
typedef std::string statementType;
typedef std::string partialMatch;
typedef std::string procedure;

class PKB {
 private:
  std::unique_ptr<AssignStore> assignStore;
  std::unique_ptr<VariableStore> variableStore;
  std::unique_ptr<UsesStore> usesStore;
  std::unique_ptr<ConstantStore> constantStore;
  std::unique_ptr<StatementStore> statementStore;
  std::unique_ptr<ParentStore> parentStore;
  std::unique_ptr<FollowsStore> followsStore;
  std::unique_ptr<ModifiesStore> modifiesStore;
  std::unique_ptr<ProcedureStore> procedureStore;
  std::unique_ptr<CallStore> callStore;

 public:
  PKB();

  // AssignStore methods

  /**
  * @brief Sets assignment information in the program.
  *
  * This method sets assignment information, including mappings of statement numbers to sets of possible combinations
  * (numRHSMap) and statement numbers to variables (numLHSMap) in the program.
  *
  * @param numRHSMap An unordered map of statement numbers to sets of possible combinations for the right-hand side of assignments.
  * @param numLHSMap An unordered map of statement numbers to variables for the left-hand side of assignments.
  */
  void setAssignments(std::unordered_map<statementNumber, std::unordered_set<partialMatch>> partialRHSMap,
                      std::unordered_map<statementNumber, variable> numLHSMap);

  void setAssignments(std::unordered_map<statementNumber, std::unordered_set<partialMatch>> partialRHSMap,
                        std::unordered_map<statementNumber, full> fullRHSMap,
                        std::unordered_map<statementNumber, variable> numLHSMap);

    std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPairPartial(partialMatch partial);
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPairFull(full full);
    std::unordered_set<statementNumber> getAssignsWcF(Wildcard lhs, full rhs);
    std::unordered_set<statementNumber> getAssignsFF(full lhs, full rhs);

  /**
  * @brief Retrieves all assignment statement numbers in the program.
  *
  * This method returns an unordered set containing all statement numbers corresponding to assignment statements in the program.
  *
  * @return An unordered set of statement numbers representing assignment statements.
  */
  std::unordered_set<statementNumber> getAllAssigns();

  /**
  * @brief Retrieves pairs of statement numbers and variables for assignment statements that partially match a given pattern.
  *
  * This method returns an unordered set of pairs, where each pair consists of a statement number and a variable,
  * for assignment statements that partially match the given pattern (partialMatch).
  *
  * @param partial The partial match pattern for assignment statements.
  * @return An unordered set of pairs representing assignment statements that partially match the pattern.
  */
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPair(partialMatch partial);

  /**
  * @brief Retrieves pairs of statement numbers and variables for assignment statements that match a wildcard pattern.
  *
  * This method returns an unordered set of pairs, where each pair consists of a statement number and a variable,
  * for assignment statements that match the given wildcard pattern (Wildcard).
  *
  * @param wildcard The wildcard pattern for assignment statements.
  * @return An unordered set of pairs representing assignment statements that match the wildcard pattern.
  */
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPair(Wildcard wildcard);

  /**
  * @brief Retrieves assignment statement numbers that match a pattern for the left-hand side (LHS) and partial match for the right-hand side (RHS).
  *
  * This method returns an unordered set of statement numbers for assignment statements that match the pattern for the LHS
  * and partially match the pattern for the RHS.
  *
  * @param lhs The pattern for the left-hand side of assignment statements.
  * @param rhs The partial match pattern for the right-hand side of assignment statements.
  * @return An unordered set of statement numbers representing matching assignment statements.
  */
  std::unordered_set<statementNumber> getAssigns(Wildcard lhs, partialMatch rhs);

  /**
  * @brief Retrieves assignment statement numbers that match wildcard patterns for both the left-hand side (LHS) and right-hand side (RHS).
  *
  * This method returns an unordered set of statement numbers for assignment statements that match wildcard patterns for both
  * the LHS and RHS.
  *
  * @param lhs The wildcard pattern for the left-hand side of assignment statements.
  * @param rhs The wildcard pattern for the right-hand side of assignment statements.
  * @return An unordered set of statement numbers representing matching assignment statements.
  */
  std::unordered_set<statementNumber> getAssigns(Wildcard lhs, Wildcard rhs);

  /**
  * @brief Retrieves assignment statement numbers that partially match patterns for both the left-hand side (LHS) and right-hand side (RHS).
  *
  * This method returns an unordered set of statement numbers for assignment statements that partially match patterns for both
  * the LHS and RHS.
  *
  * @param lhs The partial match pattern for the left-hand side of assignment statements.
  * @param rhs The partial match pattern for the right-hand side of assignment statements.
  * @return An unordered set of statement numbers representing matching assignment statements.
  */
  std::unordered_set<statementNumber> getAssigns(partialMatch lhs, partialMatch rhs);

  /**
  * @brief Retrieves assignment statement numbers that partially match a pattern for the left-hand side (LHS) and match a wildcard pattern for the right-hand side (RHS).
  *
  * This method returns an unordered set of statement numbers for assignment statements that partially match a pattern for the LHS
  * and match a wildcard pattern for the RHS.
  *
  * @param lhs The partial match pattern for the left-hand side of assignment statements.
  * @param rhs The wildcard pattern for the right-hand side of assignment statements.
  * @return An unordered set of statement numbers representing matching assignment statements.
  */
  std::unordered_set<statementNumber> getAssigns(partialMatch lhs, Wildcard rhs);


  // ProcedureStore methods

  /**
  * @brief Adds a set of procedures to the program's procedure store.
  *
  * This method adds a set of procedures to the program's procedure store.
  *
  * @param procedures A set of procedures to be added to the program's procedure store.
  */
  void addProcedures(std::set<procedure> procedures);

  /**
  * @brief Adds procedures and thier respective start numbers and line numbers to the program's procedure store.
  * 
  * This method adds procedures and thier respective start numbers and line numbers to the program's procedure store.
  * 
  * @param procedures An unordered map of procedures to pairs of start numbers and line numbers.
  */
  void addProcedures(std::unordered_map<procedure, std::pair<int, int>> procedures);

  /**
  * @brief Retrieves all procedures stored in the program.
  *
  * This method returns an unordered set containing all procedures stored in the program's procedure store.
  *
  * @return An unordered set of procedures stored in the program.
  */
  std::unordered_set<procedure> getProcedures();

  /**
  * @brief Retrieves the start number and end number for a specific procedure.
  * 
  * This method returns a pair of integers representing the start number and end number for the specified procedure.
  * 
  * @param proc The procedure for which to retrieve the start number and end number.
  * 
  * @return A pair of integers representing the start number and end number for the specified procedure.
  */
  std::pair<int, int> getProcedureRange(procedure proc);

  /**
  * @brief Adds a set of variables to the program's variable store.
  *
  * This method adds a set of variables to the program's variable store.
  *
  * @param variables An unordered set of variables to be added to the program's variable store.
  */
  void addVariables(std::unordered_set<variable> variables);

  /**
  * @brief Retrieves all variables stored in the program.
  *
  * This method returns an unordered set containing all variables stored in the program's variable store.
  *
  * @return An unordered set of variables stored in the program.
  */
  std::unordered_set<variable> getVariables();


  // UsesStore methods

  /**
  * @brief Stores uses information in the program.
  *
  * This method stores uses information, including mappings of statement numbers to sets of used variables (varUsesMap).
  *
  * @param varUsesMap An unordered map of statement numbers to sets of variables that are used.
  */
  void storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap);

  void storeUsesProcedures(std::unordered_map<procedure, std::pair<int, int>> procedures,
      std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar);

  /**
  * @brief Checks if a specific statement uses a given variable.
  *
  * This method checks whether the statement specified by 'lineNumber' uses the variable 'variableName'.
  *
  * @param lineNumber The statement number to check for variable usage.
  * @param variableName The name of the variable to check for usage.
  * @return true if the statement uses the variable, false otherwise.
  */
  bool isUses(statementNumber lineNumber, variable variableName);

  /**
  * @brief Checks if a specific statement uses variables matching a wildcard pattern.
  *
  * This method checks whether the statement specified by 'lineNumber' uses variables that match the given wildcard pattern.
  *
  * @param lineNumber The statement number to check for variable usage.
  * @param wildcard A wildcard pattern to match against used variables.
  * @return true if the statement uses matching variables, false otherwise.
  */
  bool isUses(statementNumber lineNumber, Wildcard wildcard);

  /**
  * @brief Retrieves variables used by a specific statement.
  *
  * This method returns an unordered set of variables used by the statement specified by 'line'.
  *
  * @param line The statement number for which to retrieve used variables.
  * @return An unordered set of variables used by the specified statement.
  */
  std::unordered_set<variable> uses(statementNumber line);

  /**
  * @brief Retrieves statements that use a specific variable of a given type.
  *
  * This method returns an unordered set of statement numbers for statements that use the variable 'variableName' of the specified type (StmtEntity).
  *
  * @param type The type of statement for which to search for variable usage.
  * @param variableName The name of the variable to search for in statement usage.
  * @return An unordered set of statement numbers that use the specified variable of the given type.
  */
  std::unordered_set<statementNumber> uses(StmtEntity type, variable variableName);

  /**
  * @brief Retrieves statements that use variables matching a wildcard pattern of a given type.
  *
  * This method returns an unordered set of statement numbers for statements that use variables matching the given wildcard pattern (Wildcard) of the specified type (StmtEntity).
  *
  * @param type The type of statement for which to search for variable usage.
  * @param wildcard A wildcard pattern to match against used variables.
  * @return An unordered set of statement numbers that use matching variables of the given type.
  */
  std::unordered_set<statementNumber> uses(StmtEntity type, Wildcard wildcard);

  /**
  * @brief Retrieves pairs of statement numbers and variables for statements of a given type that represent usage relationships.
  *
  * This method returns an unordered set of pairs, where each pair consists of a statement number and a variable, for statements of the specified type (StmtEntity) that represent usage relationships.
  *
  * @param type The type of statement for which to retrieve usage relationships.
  * @return An unordered set of pairs representing usage relationships for statements of the specified type.
  */
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> uses(StmtEntity type);

  bool isUses(procedure procedure, Wildcard wildcard);

  std::unordered_set<variable> uses(procedure procedure);

  bool isUses(procedure procedure, variable variableName);

  std::unordered_set<procedure> usesProcedure(Wildcard wildcard);

  std::unordered_set<procedure> usesProcedure(variable variableName);

  std::unordered_set<std::pair<procedure, variable>, PairHash> usesProcedure();

  // ModifiesStore methods

  /**
  * @brief Stores modifies information in the program.
  *
  * This method stores modifies information, including mappings of statement numbers to modified variables (varModifiesMap).
  *
  * @param varModifiesMap An unordered map of statement numbers to variables that are modified.
  */
  void storeModifies(std::unordered_map<statementNumber, variable> varModifiesMap);

  void storeModifiesProcedures(std::unordered_map<procedure, std::pair<int, int>> procedures,
      std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar);

  /**
  * @brief Checks if a specific statement modifies a given variable.
  *
  * This method checks whether the statement specified by 'lineNumber' modifies the variable 'variableName'.
  *
  * @param lineNumber The statement number to check for variable modification.
  * @param variableName The name of the variable to check for modification.
  * @return true if the statement modifies the variable, false otherwise.
  */
  bool isModifies(statementNumber lineNumber, variable variableName);

  /**
  * @brief Checks if a specific statement modifies variables matching a wildcard pattern.
  *
  * This method checks whether the statement specified by 'lineNumber' modifies variables that match the given wildcard pattern.
  *
  * @param lineNumber The statement number to check for variable modification.
  * @param wildcard A wildcard pattern to match against modified variables.
  * @return true if the statement modifies matching variables, false otherwise.
  */
  bool isModifies(statementNumber lineNumber, Wildcard wildcard);

  /**
  * @brief Retrieves variables modified by a specific statement.
  *
  * This method returns an unordered set of variables modified by the statement specified by 'line'.
  *
  * @param line The statement number for which to retrieve modified variables.
  * @return An unordered set of variables modified by the specified statement.
  */
  std::unordered_set<variable> modifies(statementNumber line);

  /**
  * @brief Retrieves statements that modify a specific variable of a given type.
  *
  * This method returns an unordered set of statement numbers for statements that modify the variable 'variableName' of the specified type (StmtEntity).
  *
  * @param type The type of statement for which to search for variable modification.
  * @param variableName The name of the variable to search for in statement modification.
  * @return An unordered set of statement numbers that modify the specified variable of the given type.
  */
  std::unordered_set<statementNumber> modifies(StmtEntity type, variable variableName);

  /**
  * @brief Retrieves statements that modify variables matching a wildcard pattern of a given type.
  *
  * This method returns an unordered set of statement numbers for statements that modify variables matching the given wildcard pattern (Wildcard) of the specified type (StmtEntity).
  *
  * @param type The type of statement for which to search for variable modification.
  * @param wildcard A wildcard pattern to match against modified variables.
  * @return An unordered set of statement numbers that modify matching variables of the given type.
  */
  std::unordered_set<statementNumber> modifies(StmtEntity type, Wildcard wildcard);

  /**
  * @brief Retrieves pairs of statement numbers and variables for statements of a given type that represent modification relationships.
  *
  * This method returns an unordered set of pairs, where each pair consists of a statement number and a variable, for statements of the specified type (StmtEntity) that represent modification relationships.
  *
  * @param type The type of statement for which to retrieve modification relationships.
  * @return An unordered set of pairs representing modification relationships for statements of the specified type.
  */
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> modifies(StmtEntity type);

  bool isModifies(procedure procedure, Wildcard wildcard);

  std::unordered_set<variable> modifies(procedure procedure);

  bool isModifies(procedure procedure, variable variableName);

  std::unordered_set<procedure> modifiesProcedure(Wildcard wildcard);

  std::unordered_set<procedure> modifiesProcedure(variable variableName);

  std::unordered_set<std::pair<procedure, variable>, PairHash> modifiesProcedure();

  // ConstantStore methods

  /**
  * @brief Adds a set of constants to the program's constant store.
  *
  * This method adds a set of constants to the program's constant store.
  *
  * @param constants An unordered set of constants to be added to the program's constant store.
  */
  void addConstants(std::unordered_set<constant> constants);

  /**
  * @brief Retrieves all constants stored in the program.
  *
  * This method returns an unordered set containing all constants stored in the program's constant store.
  *
  * @return An unordered set of constants stored in the program.
  */
  std::unordered_set<constant> getConstants();


  // StatementStore methods

  /**
  * @brief Adds statement information to the program.
  *
  * This method adds statement information, including mappings of statement numbers to statement entities (typeMap) in the program.
  *
  * @param typeMap An unordered map of statement numbers to statement entities.
  */
  void addStatements(std::unordered_map<statementNumber, StmtEntity> typeMap);

  /**
  * @brief Retrieves statement numbers of a specific type.
  *
  * This method returns an unordered set containing statement numbers of the specified type (StmtEntity) stored in the program.
  *
  * @param type The type of statement for which to retrieve statement numbers.
  * @return An unordered set of statement numbers of the specified type.
  */
  std::unordered_set<statementNumber> getStatements(StmtEntity type);


  // ParentStore methods

  /**
  * @brief Stores parent relationship information in the program.
  *
  * This method stores parent relationship information, including mappings of statement numbers to sets of parent statements (map).
  *
  * @param map An unordered map of statement numbers to sets of parent statement numbers.
  */
  void storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map);

  /**
  * @brief Retrieves parent statement numbers based on a wildcard pattern and statement type.
  *
  * This method returns an unordered set of parent statement numbers that match a wildcard pattern (wildcard) and the specified statement type (entity).
  *
  * @param wildcard A wildcard pattern for statement numbers.
  * @param entity The type of statement for which to search for parent statements.
  * @return An unordered set of parent statement numbers that match the pattern and type.
  */
  std::unordered_set<statementNumber> parent(Wildcard wildcard, StmtEntity entity);

  /**
  * @brief Retrieves parent statement numbers based on a specific statement number and statement type.
  *
  * This method returns an unordered set of parent statement numbers for the specified statement number (num) and the specified statement type (entity).
  *
  * @param num The statement number for which to find parent statements.
  * @param entity The type of statement for which to search for parent statements.
  * @return An unordered set of parent statement numbers for the specified statement number and type.
  */
  std::unordered_set<statementNumber> parent(statementNumber num, StmtEntity entity);

  /**
  * @brief Retrieves parent statement numbers based on a statement type and a specific statement number.
  *
  * This method returns an unordered set of parent statement numbers for the specified statement type (entity) and the given statement number (num).
  *
  * @param entity The type of statement for which to search for parent statements.
  * @param num The statement number for which to find parent statements.
  * @return An unordered set of parent statement numbers for the specified type and statement number.
  */
  std::unordered_set<statementNumber> parent(StmtEntity entity, statementNumber num);

  /**
  * @brief Retrieves parent statement numbers based on a statement type and a wildcard pattern.
  *
  * This method returns an unordered set of parent statement numbers for the specified statement type (entity) and a wildcard pattern (wildcard).
  *
  * @param entity The type of statement for which to search for parent statements.
  * @param wildcard A wildcard pattern for statement numbers.
  * @return An unordered set of parent statement numbers for the specified type and wildcard pattern.
  */
  std::unordered_set<statementNumber> parent(StmtEntity entity, Wildcard wildcard);

  /**
  * @brief Retrieves parent statement numbers based on a wildcard pattern and a specific statement number.
  *
  * This method returns an unordered set of parent statement numbers that match a wildcard pattern (wildcard) and the specified statement number (statement).
  *
  * @param wildcard A wildcard pattern for statement numbers.
  * @param statement The specific statement number for which to search for parent statements.
  * @return An unordered set of parent statement numbers that match the pattern and the specific statement number.
  */
  std::unordered_set<statementNumber> parent(Wildcard wildcard, statementNumber statement);

  /**
  * @brief Retrieves parent statement numbers based on a specific statement number and a wildcard pattern.
  *
  * This method returns an unordered set of parent statement numbers for the specified statement number (statement) and a wildcard pattern (wildcard).
  *
  * @param statement The specific statement number for which to find parent statements.
  * @param wildcard A wildcard pattern for statement numbers.
  * @return An unordered set of parent statement numbers for the specific statement number and wildcard pattern.
  */
  std::unordered_set<statementNumber> parent(statementNumber statement, Wildcard wildcard);

  /**
  * @brief Retrieves pairs of parent and child statement numbers based on statement types.
  *
  * This method returns an unordered set of pairs, where each pair consists of a parent statement number and a child statement number,
  * for specified statement types (entity and entity2) representing parent-child relationships.
  *
  * @param entity The first type of statement for which to retrieve parent-child relationships.
  * @param entity2 The second type of statement for which to retrieve parent-child relationships.
  * @return An unordered set of pairs representing parent-child relationships for the specified statement types.
  */
  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
  parent(StmtEntity entity, StmtEntity entity2);

  /**
  * @brief Checks if one statement is the parent of another statement.
  *
  * This method checks if the statement specified by 'parent' is the parent of the statement specified by 'child'.
  *
  * @param parent The statement number to check as the parent.
  * @param child The statement number to check as the child.
  * @return true if 'parent' is the parent of 'child', false otherwise.
  */
  bool isParent(statementNumber parent, statementNumber child);

  /**
  * @brief Checks if a statement, specified by its number, is a parent of statements matching a wildcard pattern and a given type.
  *
  * This method checks if the statement specified by 'parent' is a parent of statements matching a wildcard pattern (wildcard)
  * and the specified statement type (entity).
  *
  * @param parent The statement number to check as the parent.
  * @param wildcard A wildcard pattern for child statements.
  * @return true if 'parent' is a parent of matching child statements, false otherwise.
  */
  bool isParent(statementNumber parent, Wildcard wildcard);

  /**
  * @brief Checks if a statement, specified by its number, is a parent of statements matching a wildcard pattern and a given type.
  *
  * This method checks if the statement specified by 'parent' is a parent of statements matching a wildcard pattern (wildcard)
  * and the specified statement type (entity).
  *
  * @param wildcard A wildcard pattern for parent statements.
  * @param child The statement number to check as the child.
  * @return true if 'parent' is a parent of matching child statements, false otherwise.
  */
  bool isParent(Wildcard wildcard, statementNumber child);

  /**
  * @brief Checks if statements matching a wildcard pattern are parents of statements matching another wildcard pattern.
  *
  * This method checks if statements matching the first wildcard pattern (wildcard) are parents of statements matching
  * the second wildcard pattern (wildcard2).
  *
  * @param wildcard A wildcard pattern for parent statements.
  * @param wildcard2 A wildcard pattern for child statements.
  * @return true if statements matching the first wildcard pattern are parents of statements matching the second wildcard pattern, false otherwise.
  */
  bool isParent(Wildcard wildcard, Wildcard wildcard2);

  /**
  * @brief Retrieves all descendant statements of a specific statement that match a wildcard pattern.
  *
  * This method returns an unordered set of descendant statement numbers that match the specified wildcard pattern (wildcard).
  * Descendants are statements that are indirectly or directly nested within the given statement number (statement).
  *
  * @param statement The statement number for which to find descendant statements.
  * @param wildcard A wildcard pattern for descendant statements.
  * @return An unordered set of descendant statement numbers that match the wildcard pattern.
  */
  std::unordered_set<statementNumber> parentStar(statementNumber statement, Wildcard wildcard);

  /**
  * @brief Retrieves all ancestor statements of a specific statement that match a wildcard pattern.
  *
  * This method returns an unordered set of ancestor statement numbers that match the specified wildcard pattern (wildcard).
  * Ancestors are statements that directly or indirectly enclose the given statement number (statement).
  *
  * @param wildcard A wildcard pattern for ancestor statements.
  * @param statement The statement number for which to find ancestor statements.
  * @return An unordered set of ancestor statement numbers that match the wildcard pattern.
  */
  std::unordered_set<statementNumber> parentStar(Wildcard wildcard, statementNumber statement);

  /**
  * @brief Retrieves all descendant statements of statements matching a wildcard pattern and a specified type.
  *
  * This method returns an unordered set of descendant statement numbers that match the specified wildcard pattern (wildcard)
  * and the specified statement type (entity).
  * Descendants are statements that are indirectly or directly nested within the matching statements.
  *
  * @param wildcard A wildcard pattern for matching statements.
  * @param entity The type of statement for which to search for descendant statements.
  * @return An unordered set of descendant statement numbers that match the wildcard pattern and type.
  */
  std::unordered_set<statementNumber> parentStar(Wildcard wildcard, StmtEntity entity);

  /**
  * @brief Retrieves all ancestor statements of a specific statement number and a specified type that match a wildcard pattern.
  *
  * This method returns an unordered set of ancestor statement numbers that match the specified wildcard pattern (wildcard)
  * and the specified statement type (entity).
  * Ancestors are statements that directly or indirectly enclose the given statement number (num).
  *
  * @param num The statement number for which to find ancestor statements.
  * @param entity The type of statement for which to search for ancestor statements.
  * @return An unordered set of ancestor statement numbers that match the wildcard pattern and type.
  */
  std::unordered_set<statementNumber> parentStar(statementNumber num, StmtEntity entity);

  /**
  * @brief Retrieves all descendant statements of statements matching a specified type and a specific statement number.
  *
  * This method returns an unordered set of descendant statement numbers for the specified statement type (entity)
  * and the given statement number (num).
  * Descendants are statements that are indirectly or directly nested within the matching statements.
  *
  * @param entity The type of statement for which to search for descendant statements.
  * @param num The statement number for which to find descendant statements.
  * @return An unordered set of descendant statement numbers that match the specified type and statement number.
  */
  std::unordered_set<statementNumber> parentStar(StmtEntity entity, statementNumber num);

  /**
  * @brief Retrieves all descendant statements of statements matching a specified type and a wildcard pattern.
  *
  * This method returns an unordered set of descendant statement numbers for the specified statement type (entity)
  * and the given wildcard pattern (wildcard).
  * Descendants are statements that are indirectly or directly nested within the matching statements.
  *
  * @param entity The type of statement for which to search for descendant statements.
  * @param wildcard A wildcard pattern for matching statements.
  * @return An unordered set of descendant statement numbers that match the specified type and wildcard pattern.
  */
  std::unordered_set<statementNumber> parentStar(StmtEntity entity, Wildcard wildcard);

  /**
  * @brief Retrieves pairs of ancestor and descendant statement numbers based on statement types.
  *
  * This method returns an unordered set of pairs, where each pair consists of an ancestor statement number and a descendant statement number,
  * for specified statement types (entity and entity2) representing ancestor-descendant relationships.
  *
  * @param entity The first type of statement for which to retrieve ancestor-descendant relationships.
  * @param entity2 The second type of statement for which to retrieve ancestor-descendant relationships.
  * @return An unordered set of pairs representing ancestor-descendant relationships for the specified statement types.
  */
  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
  parentStar(StmtEntity entity, StmtEntity entity2);

  /**
  * @brief Checks if one statement is an ancestor of another statement.
  *
  * This method checks if the statement specified by 'parent' is an ancestor of the statement specified by 'child'.
  *
  * @param parent The statement number to check as the ancestor.
  * @param child The statement number to check as the descendant.
  * @return true if 'parent' is an ancestor of 'child', false otherwise.
  */
  bool isParentStar(statementNumber parent, statementNumber child);

  /**
  * @brief Checks if a statement, specified by its number, is an ancestor of statements matching a wildcard pattern and a given type.
  *
  * This method checks if the statement specified by 'parent' is an ancestor of statements matching a wildcard pattern (wildcard)
  * and the specified statement type (entity).
  *
  * @param parent The statement number to check as the ancestor.
  * @param wildcard A wildcard pattern for descendant statements.
  * @return true if 'parent' is an ancestor of matching descendant statements, false otherwise.
  */
  bool isParentStar(statementNumber parent, Wildcard wildcard);

  /**
  * @brief Checks if statements matching a wildcard pattern are ancestors of a statement specified by its number.
  *
  * This method checks if statements matching the wildcard pattern (wildcard) are ancestors of the statement specified by 'child'.
  *
  * @param wildcard A wildcard pattern for ancestor statements.
  * @param child The statement number to check as the descendant.
  * @return true if matching ancestor statements are ancestors of 'child', false otherwise.
  */
  bool isParentStar(Wildcard wildcard, statementNumber child);

  /**
  * @brief Checks if statements matching a wildcard pattern are ancestors of statements matching another wildcard pattern.
  *
  * This method checks if statements matching the first wildcard pattern (wildcard) are ancestors of statements matching
  * the second wildcard pattern (wildcard2).
  *
  * @param wildcard A wildcard pattern for ancestor statements.
  * @param wildcard2 A wildcard pattern for descendant statements.
  * @return true if matching ancestor statements are ancestors of matching descendant statements, false otherwise.
  */
  bool isParentStar(Wildcard wildcard, Wildcard wildcard2);


  // FollowStore Method

  /**
  * @brief Stores follows relationship information in the program.
  *
  * This method stores follows relationship information, including mappings of statement numbers to their following statement numbers (map).
  *
  * @param map An unordered map of statement numbers to their following statement numbers.
  */
  void storeFollows(std::unordered_map<statementNumber, statementNumber> map);

  /**
  * @brief Retrieves statements following a wildcard pattern and a specified statement type.
  *
  * This method returns an unordered set of statement numbers that follow the specified wildcard pattern (wildcard)
  * and the specified statement type (entity).
  *
  * @param wildcard A wildcard pattern for preceding statements.
  * @param entity The type of statement for which to search for following statements.
  * @return An unordered set of statement numbers that match the wildcard pattern and type.
  */
  std::unordered_set<statementNumber> follows(Wildcard wildcard, StmtEntity entity);

  /**
  * @brief Retrieves statements following a specific statement number and a specified statement type.
  *
  * This method returns an unordered set of statement numbers that follow the specified statement number (num)
  * and the specified statement type (entity).
  *
  * @param num The statement number for which to find following statements.
  * @param entity The type of statement for which to search for following statements.
  * @return An unordered set of statement numbers that follow the specified statement number and type.
  */
  std::unordered_set<statementNumber> follows(statementNumber num, StmtEntity entity);

  /**
  * @brief Retrieves statements followed by a specific statement number and a specified statement type.
  *
  * This method returns an unordered set of statement numbers that are followed by the specified statement number (num)
  * and the specified statement type (entity).
  *
  * @param entity The type of statement for which to search for preceding statements.
  * @param num The statement number for which to find preceding statements.
  * @return An unordered set of statement numbers that are followed by the specified statement number and type.
  */
  std::unordered_set<statementNumber> follows(StmtEntity entity, statementNumber num);

  /**
  * @brief Retrieves statements followed by statements matching a specified type and a wildcard pattern.
  *
  * This method returns an unordered set of statement numbers that are followed by statements matching the specified statement type (entity)
  * and the given wildcard pattern (wildcard).
  *
  * @param entity The type of statement for which to search for preceding statements.
  * @param wildcard A wildcard pattern for following statements.
  * @return An unordered set of statement numbers that are followed by matching statements of the specified type.
  */
  std::unordered_set<statementNumber> follows(StmtEntity entity, Wildcard wildcard);

  /**
  * @brief Retrieves pairs of preceding and following statement numbers based on statement types.
  *
  * This method returns an unordered set of pairs, where each pair consists of a preceding statement number and a following statement number,
  * for specified statement types (entity1 and entity2) representing follows relationships.
  *
  * @param entity1 The type of statement for which to retrieve preceding statements.
  * @param entity2 The type of statement for which to retrieve following statements.
  * @return An unordered set of pairs representing follows relationships for the specified statement types.
  */
  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
  follows(StmtEntity entity1, StmtEntity entity2);

  /**
  * @brief Checks if one statement follows another statement.
  *
  * This method checks if the statement specified by 'statement1' follows the statement specified by 'statement2'.
  *
  * @param statement1 The statement number to check as the following statement.
  * @param statement2 The statement number to check as the preceding statement.
  * @return true if 'statement1' follows 'statement2', false otherwise.
  */
  bool isFollow(statementNumber statement1, statementNumber statement2);

  /**
  * @brief Checks if a statement, specified by its number, follows statements matching a wildcard pattern and a given type.
  *
  * This method checks if the statement specified by 'statement1' follows statements matching a wildcard pattern (wildcard)
  * and the specified statement type (entity).
  *
  * @param statement1 The statement number to check as the following statement.
  * @param wildcard A wildcard pattern for preceding statements.
  * @return true if 'statement1' follows matching preceding statements, false otherwise.
  */
  bool isFollow(statementNumber statement1, Wildcard wildcard);

  /**
  * @brief Checks if statements matching a wildcard pattern are followed by a statement specified by its number.
  *
  * This method checks if statements matching the wildcard pattern (wildcard) are followed by the statement specified by 'statement1'.
  *
  * @param wildcard A wildcard pattern for following statements.
  * @param statement1 The statement number to check as the following statement.
  * @return true if matching preceding statements are followed by 'statement1', false otherwise.
  */
  bool isFollow(Wildcard wildcard, statementNumber statement1);

  /**
  * @brief Checks if statements matching a wildcard pattern are followed by statements matching another wildcard pattern.
  *
  * This method checks if statements matching the first wildcard pattern (wildcard) are followed by statements matching
  * the second wildcard pattern (wildcard2).
  *
  * @param wildcard A wildcard pattern for preceding statements.
  * @param wildcard2 A wildcard pattern for following statements.
  * @return true if matching preceding statements are followed by matching following statements, false otherwise.
  */
  bool isFollow(Wildcard wildcard, Wildcard wildcard2);

  /**
  * @brief Retrieves all statements that follow a wildcard pattern and match a specified statement type.
  *
  * This method returns an unordered set of statement numbers that follow the specified wildcard pattern (wildcard)
  * and match the specified statement type (entity).
  *
  * @param wildcard A wildcard pattern for preceding statements.
  * @param entity The type of statement for which to search for following statements.
  * @return An unordered set of statement numbers that follow the wildcard pattern and match the type.
  */
  std::unordered_set<statementNumber> followStar(Wildcard wildcard, StmtEntity entity);

  /**
  * @brief Retrieves all statements that are followed by a specific statement number and match a specified statement type.
  *
  * This method returns an unordered set of statement numbers that are followed by the specified statement number (num)
  * and match the specified statement type (entity).
  *
  * @param num The statement number for which to find following statements.
  * @param entity The type of statement for which to search for following statements.
  * @return An unordered set of statement numbers that are followed by the specified statement number and match the type.
  */
  std::unordered_set<statementNumber> followStar(statementNumber num, StmtEntity entity);

  /**
  * @brief Retrieves all statements that are followed by statements matching a specified type and a specific statement number.
  *
  * This method returns an unordered set of statement numbers that are followed by statements matching the specified statement type (entity)
  * and the given statement number (num).
  *
  * @param entity The type of statement for which to search for preceding statements.
  * @param num The statement number for which to find preceding statements.
  * @return An unordered set of statement numbers that are followed by matching statements of the specified type.
  */
  std::unordered_set<statementNumber> followStar(StmtEntity entity, statementNumber num);

  /**
  * @brief Retrieves all statements that are followed by statements matching a specified type and a wildcard pattern.
  *
  * This method returns an unordered set of statement numbers that are followed by statements matching the specified statement type (entity)
  * and the given wildcard pattern (wildcard).
  *
  * @param entity The type of statement for which to search for preceding statements.
  * @param wildcard A wildcard pattern for following statements.
  * @return An unordered set of statement numbers that are followed by matching statements of the specified type.
  */
  std::unordered_set<statementNumber> followStar(StmtEntity entity, Wildcard wildcard);

  /**
  * @brief Retrieves pairs of preceding and following statement numbers based on statement types.
  *
  * This method returns an unordered set of pairs, where each pair consists of a preceding statement number and a following statement number,
  * for specified statement types (entity1 and entity2) representing follow-star relationships.
  *
  * @param entity1 The type of statement for which to retrieve preceding statements.
  * @param entity2 The type of statement for which to retrieve following statements.
  * @return An unordered set of pairs representing follow-star relationships for the specified statement types.
  */
  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
  followStar(StmtEntity entity1, StmtEntity entity2);

  /**
  * @brief Checks if one statement follows another statement in the follow-star relationship.
  *
  * This method checks if the statement specified by 'statement1' follows the statement specified by 'statement2'
  * in the follow-star relationship, considering all possible transitive following relationships.
  *
  * @param statement1 The statement number to check as the following statement.
  * @param statement2 The statement number to check as the preceding statement.
  * @return true if 'statement1' follows 'statement2' in the follow-star relationship, false otherwise.
  */
  bool isFollowStar(statementNumber statement1, statementNumber statement2);

  /**
  * @brief Checks if a statement, specified by its number, follows statements matching a wildcard pattern and a given type
  * in the follow-star relationship.
  *
  * This method checks if the statement specified by 'statement1' follows statements matching a wildcard pattern (wildcard)
  * and the specified statement type (entity) in the follow-star relationship, considering all possible transitive following relationships.
  *
  * @param statement1 The statement number to check as the following statement.
  * @param wildcard A wildcard pattern for preceding statements.
  * @return true if 'statement1' follows matching preceding statements in the follow-star relationship, false otherwise.
  */
  bool isFollowStar(statementNumber statement1, Wildcard wildcard);

  /**
  * @brief Checks if statements matching a wildcard pattern are followed by a statement specified by its number
  * in the follow-star relationship.
  *
  * This method checks if statements matching the wildcard pattern (wildcard) are followed by the statement specified by 'statement1'
  * in the follow-star relationship, considering all possible transitive following relationships.
  *
  * @param wildcard A wildcard pattern for following statements.
  * @param statement1 The statement number to check as the following statement.
  * @return true if matching preceding statements are followed by 'statement1' in the follow-star relationship, false otherwise.
  */
  bool isFollowStar(Wildcard wildcard, statementNumber statement1);

  /**
  * @brief Checks if statements matching a wildcard pattern are followed by statements matching another wildcard pattern
  * in the follow-star relationship.
  *
  * This method checks if statements matching the first wildcard pattern (wildcard) are followed by statements matching
  * the second wildcard pattern (wildcard2) in the follow-star relationship, considering all possible transitive following relationships.
  *
  * @param wildcard A wildcard pattern for preceding statements.
  * @param wildcard2 A wildcard pattern for following statements.
  * @return true if matching preceding statements are followed by matching following statements in the follow-star relationship, false otherwise.
  */
  bool isFollowStar(Wildcard wildcard, Wildcard wildcard2);

  // CallStore methods

  void storeCalls(std::unordered_map<procedure, std::unordered_set<procedure>> callTable);

  std::unordered_map<procedure, std::unordered_set<procedure>> getCallStar();

  PKB(const PKB&) = delete;
  PKB& operator=(const PKB&) = delete;
};
