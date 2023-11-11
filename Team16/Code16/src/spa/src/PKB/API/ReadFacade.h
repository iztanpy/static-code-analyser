#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <set>
#include <utility>
#include "PKB/PKB.h"
#include "utils/entity_types.h"

typedef std::string variable;
typedef int statementNumber;
typedef std::string constant;
typedef std::string procedure;

class ReadFacade {
 private:
  PKB& pkb;

 public:
  explicit ReadFacade(PKB& pkb);
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPair(ExactExpr rhs);
  std::unordered_set<statementNumber> getAssigns(Wildcard lhs, ExactExpr rhs);
  std::unordered_set<statementNumber> getAssigns(variable lhs, ExactExpr rhs);

  /**
  * @brief Retrieves a set of all statement numbers corresponding to assignment statements in a program.
  *
  * @return An unordered set containing statement numbers for assignment statements.
  */
  std::unordered_set<statementNumber> getAllAssigns();

  /**
  * @brief Retrieves pairs of statement numbers and variables for assignment statements that partially match a given pattern.
  *
  * @param partial A pattern to partially match against assignment statements.
  * @return An unordered set of pairs, where each pair consists of a statement number and a variable matching the pattern.
  */
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPair(PartialExpr rhs);

  /**
   * @brief Retrieves pairs of statement numbers and variables for assignment statements that match a given wildcard pattern.
   *
   * @param wildcard A wildcard pattern to match against assignment statements.
   * @return An unordered set of pairs, where each pair consists of a statement number and a variable matching the wildcard pattern.
   */
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPair(Wildcard wildcard);

  /**
   * @brief Retrieves statement numbers for assignment statements where the left-hand side (LHS) matches a wildcard pattern,
   *        and the right-hand side (RHS) partially matches a given pattern.
   *
   * @param lhs A wildcard pattern for matching the left-hand side (LHS) of assignments.
   * @param rhs A pattern to partially match against the right-hand side (RHS) of assignments.
   * @return An unordered set containing statement numbers for matching assignments.
   */
  std::unordered_set<statementNumber> getAssigns(Wildcard lhs, PartialExpr rhs);

  /**
   * @brief Retrieves statement numbers for assignment statements where both the left-hand side (LHS) and the right-hand side (RHS)
   *        match wildcard patterns.
   *
   * @param lhs A wildcard pattern for matching the left-hand side (LHS) of assignments.
   * @param rhs A wildcard pattern for matching the right-hand side (RHS) of assignments.
   * @return An unordered set containing statement numbers for matching assignments.
   */
  std::unordered_set<statementNumber> getAssigns(Wildcard lhs, Wildcard rhs);

  /**
  * @brief Retrieves statement numbers for assignment statements where the left-hand side (LHS) matches a specific variable,
  *        and the right-hand side (RHS) partially matches a given pattern.
  *
  * @param lhs A specific variable to match against the left-hand side (LHS) of assignments.
  * @param rhs A pattern to partially match against the right-hand side (RHS) of assignments.
  * @return An unordered set containing statement numbers for matching assignments.
  */
  std::unordered_set<statementNumber> getAssigns(variable lhs, PartialExpr rhs);

  /**
  * @brief Retrieves statement numbers for assignment statements where the left-hand side (LHS) matches a specific variable,
  *        and the right-hand side (RHS) matches a wildcard pattern.
  *
  * @param lhs A specific variable to match against the left-hand side (LHS) of assignments.
  * @param rhs A wildcard pattern for matching the right-hand side (RHS) of assignments.
  * @return An unordered set containing statement numbers for matching assignments.
  */
  std::unordered_set<statementNumber> getAssigns(variable lhs, Wildcard rhs);

  // VariableStore methods

  /**
 * @brief Retrieves a set of all variables present in the program.
 *
 * This function returns an unordered set containing all the variables used within the program.
 *
 * @return An unordered set of variables found in the program.
 */
  std::unordered_set<variable> getVariables();

  /**
  * @brief Retrieves a set of all procedures present in the program.
  *
  * This function returns an unordered set containing all the procedures defined within the program.
  * Note that this function is related to the ProcedureStore, and it retrieves procedures from that store.
  *
  * @return An unordered set of procedures found in the program.
  */
  std::unordered_set<procedure> getProcedures();


  // UsesStore methods

  /**
  * @brief Checks if a specific statement with the given line number uses a specific variable.
  *
  * This function determines whether the statement specified by the line number uses the provided variable.
  *
  * @param lineNumber The line number of the statement to check.
  * @param variableName The variable to check for usage.
  * @return true if the statement uses the variable, false otherwise.
  */
  bool isUses(statementNumber lineNumber, variable variableName);

  /**
  * @brief Checks if any statement with the given line number uses a variable that matches a wildcard pattern.
  *
  * This function checks if any statement with the specified line number uses a variable matching the wildcard pattern.
  *
  * @param lineNumber The line number of the statement to check.
  * @param wildcard A wildcard pattern to match against variables used in the statement.
  * @return true if the statement uses a matching variable, false otherwise.
  */
  bool isUses(statementNumber lineNumber, Wildcard wildcard);

  /**
  * @brief Retrieves a set of variables used in a specific statement with the given line number.
  *
  * This function returns an unordered set containing all variables used in the statement specified by the line number.
  *
  * @param line The line number of the statement to analyze.
  * @return An unordered set of variables used in the statement.
  */
  std::unordered_set<variable> uses(statementNumber line);

  /**
  * @brief Retrieves a set of statements of a specific type that use a particular variable.
  *
  * This function returns an unordered set of statement numbers of the specified type that use the provided variable.
  *
  * @param type The type of statement to search for.
  * @param variableName The variable to check for usage in the specified statement type.
  * @return An unordered set of statement numbers where the specified type uses the variable.
  */
  std::unordered_set<statementNumber> uses(StmtEntity type, variable variableName);

  /**
  * @brief Retrieves a set of statements of a specific type that use a variable matching a wildcard pattern.
  *
  * This function returns an unordered set of statement numbers of the specified type that use a variable matching the wildcard pattern.
  *
  * @param type The type of statement to search for.
  * @param wildcard A wildcard pattern to match against variables used in the specified statement type.
  * @return An unordered set of statement numbers where the specified type uses a matching variable.
  */
  std::unordered_set<statementNumber> uses(StmtEntity type, Wildcard wildcard);

  /**
  * @brief Retrieves pairs of statement numbers and variables for statements of a specific type that use variables.
  *
  * This function returns an unordered set of pairs, where each pair consists of a statement number and a variable used in statements of the specified type.
  *
  * @param type The type of statement to search for.
  * @return An unordered set of pairs containing statement numbers and variables used in the specified statement type.
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
  * @brief Checks if a specific statement with the given line number modifies a specific variable.
  *
  * This function determines whether the statement specified by the line number modifies the provided variable.
  *
  * @param lineNumber The line number of the statement to check.
  * @param variableName The variable to check for modification.
  * @return true if the statement modifies the variable, false otherwise.
  */
  bool isModifies(statementNumber lineNumber, variable variableName);

  /**
  * @brief Checks if any statement with the given line number modifies a variable that matches a wildcard pattern.
  *
  * This function checks if any statement with the specified line number modifies a variable matching the wildcard pattern.
  *
  * @param lineNumber The line number of the statement to check.
  * @param wildcard A wildcard pattern to match against variables modified in the statement.
  * @return true if the statement modifies a matching variable, false otherwise.
  */
  bool isModifies(statementNumber lineNumber, Wildcard wildcard);

  /**
  * @brief Retrieves a set of variables modified in a specific statement with the given line number.
  *
  * This function returns an unordered set containing all variables modified in the statement specified by the line number.
  *
  * @param line The line number of the statement to analyze.
  * @return An unordered set of variables modified in the statement.
  */
  std::unordered_set<variable> modifies(statementNumber line);

  /**
  * @brief Retrieves a set of statements of a specific type that modify a particular variable.
  *
  * This function returns an unordered set of statement numbers of the specified type that modify the provided variable.
  *
  * @param type The type of statement to search for.
  * @param variableName The variable to check for modification in the specified statement type.
  * @return An unordered set of statement numbers where the specified type modifies the variable.
  */
  std::unordered_set<statementNumber> modifies(StmtEntity type, variable variableName);

  /**
  * @brief Retrieves a set of statements of a specific type that modify a variable matching a wildcard pattern.
  *
  * This function returns an unordered set of statement numbers of the specified type that modify a variable matching the wildcard pattern.
  *
  * @param type The type of statement to search for.
  * @param wildcard A wildcard pattern to match against variables modified in the specified statement type.
  * @return An unordered set of statement numbers where the specified type modifies a matching variable.
  */
  std::unordered_set<statementNumber> modifies(StmtEntity type, Wildcard wildcard);

  /**
  * @brief Retrieves pairs of statement numbers and variables for statements of a specific type that modify variables.
  *
  * This function returns an unordered set of pairs, where each pair consists of a statement number and a variable modified in statements of the specified type.
  *
  * @param type The type of statement to search for.
  * @return An unordered set of pairs containing statement numbers and variables modified in the specified statement type.
  */

  bool isModifies(procedure procedure, Wildcard wildcard);

  std::unordered_set<variable> modifies(procedure procedure);

  bool isModifies(procedure procedure, variable variableName);

  std::unordered_set<procedure> modifiesProcedure(Wildcard wildcard);

  std::unordered_set<procedure> modifiesProcedure(variable variableName);

  std::unordered_set<std::pair<procedure, variable>, PairHash> modifiesProcedure();

  std::unordered_set<std::pair<statementNumber, variable>, PairHash> modifies(StmtEntity type);

  // ConstantStore methods

  /**
  * @brief Retrieves a set of all constants present in the program.
  *
  * This function returns an unordered set containing all the constants used within the program.
  *
  * @return An unordered set of constants found in the program.
  */
  std::unordered_set<constant> getConstants();

  /**
  * @brief Retrieves a set of statement numbers of a specific type.
  *
  * This function returns an unordered set containing statement numbers for statements of the specified type.
  *
  * @param type The type of statement to retrieve.
  * @return An unordered set of statement numbers of the specified type.
  */
  std::unordered_set<statementNumber> getStatements(StmtEntity type);

  /**
  * @brief Retrieves a set of statement numbers that are parent to a specified statement, matching a wildcard pattern.
  *
  * This function returns an unordered set of statement numbers that are parent to the specified statement and match the given wildcard pattern.
  *
  * @param statement The statement number to which parent statements are sought.
  * @param wildcard A wildcard pattern to match against parent statements.
  * @return An unordered set of statement numbers that are parent to the specified statement and match the wildcard.
  */
  std::unordered_set<statementNumber> parent(statementNumber statement, Wildcard wildcard);

  /**
  * @brief Retrieves a set of statement numbers that are parents of statements of a specific type, matching a wildcard pattern.
  *
  * This function returns an unordered set of statement numbers that are parent to statements of the specified type and match the given wildcard pattern.
  *
  * @param wildcard A wildcard pattern to match against parent statements.
  * @param entity The type of statement for which parents are sought.
  * @return An unordered set of statement numbers that are parent to statements of the specified type and match the wildcard.
  */
  std::unordered_set<statementNumber> parent(Wildcard wildcard, StmtEntity entity);

  /**
  * @brief Retrieves a set of statement numbers that are parent to a specified statement of a specific type.
  *
  * This function returns an unordered set of statement numbers that are parent to the specified statement and are of the specified type.
  *
  * @param num The statement number to which parent statements are sought.
  * @param entity The type of statement for which parents are sought.
  * @return An unordered set of statement numbers that are parent to the specified statement and match the specified type.
  */
  std::unordered_set<statementNumber> parent(statementNumber num, StmtEntity entity);

  /**
  * @brief Retrieves a set of statement numbers that are parents of statements of a specific type.
  *
  * This function returns an unordered set of statement numbers that are parent to statements of the specified type.
  *
  * @param entity The type of statement for which parents are sought.
  * @param num The statement number to which parent statements are sought.
  * @return An unordered set of statement numbers that are parent to statements of the specified type.
  */
  std::unordered_set<statementNumber> parent(StmtEntity entity, statementNumber num);

  /**
  * @brief Retrieves a set of statement numbers that are parents of statements of a specific type, matching a wildcard pattern.
  *
  * This function returns an unordered set of statement numbers that are parent to statements of the specified type and match the given wildcard pattern.
  *
  * @param entity The type of statement for which parents are sought.
  * @param wildcard A wildcard pattern to match against parent statements.
  * @return An unordered set of statement numbers that are parent to statements of the specified type and match the wildcard.
  */
  std::unordered_set<statementNumber> parent(StmtEntity entity, Wildcard wildcard);

  /**
  * @brief Retrieves pairs of statement numbers that represent parent-child relationships between statements of two specific types.
  *
  * This function returns an unordered set of pairs, where each pair consists of a statement number of the first type (parent) and a statement number of the second type (child) that are in a parent-child relationship.
  *
  * @param entity The type of the first statement in the parent-child relationship.
  * @param entity2 The type of the second statement in the parent-child relationship.
  * @return An unordered set of pairs representing parent-child relationships between statements of the specified types.
  */
  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
  parent(StmtEntity entity, StmtEntity entity2);

  /**
  * @brief Checks if a specified statement is a parent of another statement.
  *
  * This function checks whether the statement specified as 'parent' is a direct parent of the statement specified as 'child'.
  *
  * @param parent The statement number of the potential parent statement.
  * @param child The statement number of the potential child statement.
  * @return true if 'parent' is a direct parent of 'child', false otherwise.
  */
  bool isParent(statementNumber parent, statementNumber child);

  /**
  * @brief Checks if a specified statement is a parent to statements matching a wildcard pattern.
  *
  * This function checks whether the statement specified as 'parent' is a direct parent to statements that match the given wildcard pattern.
  *
  * @param parent The statement number of the potential parent statement.
  * @param wildcard A wildcard pattern to match against child statements.
  * @return true if 'parent' is a direct parent to matching child statements, false otherwise.
  */
  bool isParent(statementNumber parent, Wildcard wildcard);

  /**
  * @brief Checks if statements matching a wildcard pattern are parents of a specified statement.
  *
  * This function checks whether statements that match the given wildcard pattern are direct parents of the statement specified as 'child'.
  *
  * @param wildcard A wildcard pattern to match against parent statements.
  * @param child The statement number of the potential child statement.
  * @return true if matching statements are direct parents of 'child', false otherwise.
  */
  bool isParent(Wildcard wildcard, statementNumber child);

  /**
  * @brief Checks if statements matching two wildcard patterns are in a parent-child relationship.
  *
  * This function checks whether statements that match the given wildcard patterns are in a parent-child relationship.
  *
  * @param wildcard A wildcard pattern to match against parent statements.
  * @param wildcard2 A wildcard pattern to match against child statements.
  * @return true if matching statements are in a parent-child relationship, false otherwise.
  */
  bool isParent(Wildcard wildcard, Wildcard wildcard2);

  /**
  * @brief Retrieves a set of statement numbers that are in a parent* relationship with statements matching a wildcard pattern of a specific type.
  *
  * This function returns an unordered set of statement numbers that are in a parent* relationship with statements that match the given wildcard pattern and are of the specified type.
  *
  * @param wildcard A wildcard pattern to match against parent statements.
  * @param entity The type of statement for which parent* relationships are sought.
  * @return An unordered set of statement numbers that are in a parent* relationship with matching statements of the specified type.
  */
  std::unordered_set<statementNumber> parentStar(Wildcard wildcard, StmtEntity entity);

  /**
  * @brief Retrieves a set of statement numbers that are in a parent* relationship with a specified statement of a specific type.
  *
  * This function returns an unordered set of statement numbers that are in a parent* relationship with the specified statement and are of the specified type.
  *
  * @param num The statement number to which parent* relationships are sought.
  * @param entity The type of statement for which parent* relationships are sought.
  * @return An unordered set of statement numbers that are in a parent* relationship with the specified statement and match the specified type.
  */
  std::unordered_set<statementNumber> parentStar(statementNumber num, StmtEntity entity);

  /**
  * @brief Retrieves a set of statement numbers that are in a parent* relationship with a specified statement of a specific type.
  *
  * This function returns an unordered set of statement numbers that are in a parent* relationship with the specified statement and are of the specified type.
  *
  * @param entity The type of statement for which parent* relationships are sought.
  * @param num The statement number to which parent* relationships are sought.
  * @return An unordered set of statement numbers that are in a parent* relationship with the specified statement and match the specified type.
  */
  std::unordered_set<statementNumber> parentStar(StmtEntity entity, statementNumber num);

  /**
  * @brief Retrieves a set of statement numbers that are in a parent* relationship with statements matching a wildcard pattern of a specific type.
  *
  * This function returns an unordered set of statement numbers that are in a parent* relationship with statements that match the given wildcard pattern and are of the specified type.
  *
  * @param entity The type of statement for which parent* relationships are sought.
  * @param wildcard A wildcard pattern to match against parent statements.
  * @return An unordered set of statement numbers that are in a parent* relationship with matching statements of the specified type.
  */
  std::unordered_set<statementNumber> parentStar(StmtEntity entity, Wildcard wildcard);

  /**
  * @brief Retrieves pairs of statement numbers that represent parent* relationships between statements of two specific types.
  *
  * This function returns an unordered set of pairs, where each pair consists of a statement number of the first type (parent*) and a statement number of the second type (child) that are in a parent* relationship.
  *
  * @param entity The type of the first statement in the parent* relationship.
  * @param entity2 The type of the second statement in the parent* relationship.
  * @return An unordered set of pairs representing parent* relationships between statements of the specified types.
  */
  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
  parentStar(StmtEntity entity, StmtEntity entity2);

  /**
  * @brief Checks if a specified statement is in a parent* relationship with another statement.
  *
  * This function checks whether the statement specified as 'parent' is in a parent* relationship with the statement specified as 'child'.
  *
  * @param parent The statement number of the potential parent* statement.
  * @param child The statement number of the potential child statement.
  * @return true if 'parent' is in a parent* relationship with 'child', false otherwise.
  */
  bool isParentStar(statementNumber parent, statementNumber child);

  /**
  * @brief Checks if a specified statement is in a parent* relationship with statements matching a wildcard pattern.
  *
  * This function checks whether the statement specified as 'parent' is in a parent* relationship with statements that match the given wildcard pattern.
  *
  * @param parent The statement number of the potential parent* statement.
  * @param wildcard A wildcard pattern to match against child statements.
  * @return true if 'parent' is in a parent* relationship with matching child statements, false otherwise.
  */
  bool isParentStar(statementNumber parent, Wildcard wildcard);

  /**
  * @brief Checks if statements matching a wildcard pattern are in a parent* relationship with a specified statement.
  *
  * This function checks whether statements that match the given wildcard pattern are in a parent* relationship with the statement specified as 'child'.
  *
  * @param wildcard A wildcard pattern to match against parent* statements.
  * @param child The statement number of the potential child statement.
  * @return true if matching statements are in a parent* relationship with 'child', false otherwise.
  */
  bool isParentStar(Wildcard wildcard, statementNumber child);

  /**
  * @brief Checks if statements matching two wildcard patterns are in a parent* relationship with each other.
  *
  * This function checks whether statements that match the given wildcard patterns are in a parent* relationship with each other.
  *
  * @param wildcard A wildcard pattern to match against parent* statements.
  * @param wildcard2 A wildcard pattern to match against child statements.
  * @return true if matching statements are in a parent* relationship with each other, false otherwise.
  */
  bool isParentStar(Wildcard wildcard, Wildcard wildcard2);

  /**
  * @brief Retrieves a set of statement numbers that are in a follows relationship with statements of a specific type matching a wildcard pattern.
  *
  * This function returns an unordered set of statement numbers that are in a follows relationship with statements of the specified type matching the given wildcard pattern.
  *
  * @param wildcard A wildcard pattern to match against statements in the follows relationship.
  * @param entity The type of statement for which follows relationships are sought.
  * @return An unordered set of statement numbers that are in a follows relationship with matching statements of the specified type.
  */
  std::unordered_set<statementNumber> follows(Wildcard wildcard, StmtEntity entity);

  /**
  * @brief Retrieves a set of statement numbers that are in a follows relationship with a specified statement of a specific type.
  *
  * This function returns an unordered set of statement numbers that are in a follows relationship with the specified statement and are of the specified type.
  *
  * @param num The statement number to which follows relationships are sought.
  * @param entity The type of statement for which follows relationships are sought.
  * @return An unordered set of statement numbers that are in a follows relationship with the specified statement and match the specified type.
  */
  std::unordered_set<statementNumber> follows(statementNumber num, StmtEntity entity);

  /**
  * @brief Retrieves a set of statement numbers that are in a follows relationship with statements of a specific type, matching a wildcard pattern.
  *
  * This function returns an unordered set of statement numbers that are in a follows relationship with statements of the specified type and match the given wildcard pattern.
  *
  * @param entity The type of statement for which follows relationships are sought.
  * @param num The statement number to which follows relationships are sought.
  * @return An unordered set of statement numbers that are in a follows relationship with statements of the specified type.
  */
  std::unordered_set<statementNumber> follows(StmtEntity entity, statementNumber num);

  /**
  * @brief Retrieves a set of statement numbers that are in a follows relationship with statements matching a wildcard pattern of a specific type.
  *
  * This function returns an unordered set of statement numbers that are in a follows relationship with statements of the specified type matching the given wildcard pattern.
  *
  * @param entity The type of statement for which follows relationships are sought.
  * @param wildcard A wildcard pattern to match against statements in the follows relationship.
  * @return An unordered set of statement numbers that are in a follows relationship with matching statements of the specified type.
  */
  std::unordered_set<statementNumber> follows(StmtEntity entity, Wildcard wildcard);

  /**
  * @brief Retrieves pairs of statement numbers that represent follows relationships between statements of two specific types.
  *
  * This function returns an unordered set of pairs, where each pair consists of a statement number of the first type (entity1) and a statement number of the second type (entity2) that are in a follows relationship.
  *
  * @param entity1 The type of the first statement in the follows relationship.
  * @param entity2 The type of the second statement in the follows relationship.
  * @return An unordered set of pairs representing follows relationships between statements of the specified types.
  */
  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
  follows(StmtEntity entity1, StmtEntity entity2);

  /**
  * @brief Checks if one statement follows another statement.
  *
  * This function checks whether the statement specified as 'statement1' follows the statement specified as 'statement2'.
  *
  * @param statement1 The statement number of the potential following statement.
  * @param statement2 The statement number of the potential followed statement.
  * @return true if 'statement1' follows 'statement2', false otherwise.
  */
  bool isFollow(statementNumber statement1, statementNumber statement2);

  /**
  * @brief Checks if one statement follows statements matching a wildcard pattern.
  *
  * This function checks whether the statement specified as 'statement1' follows statements that match the given wildcard pattern.
  *
  * @param statement1 The statement number of the potential following statement.
  * @param wildcard A wildcard pattern to match against followed statements.
  * @return true if 'statement1' follows matching statements, false otherwise.
  */
  bool isFollow(statementNumber statement1, Wildcard wildcard);

  /**
  * @brief Checks if statements matching a wildcard pattern are followed by a specified statement.
  *
  * This function checks whether statements that match the given wildcard pattern are followed by the statement specified as 'statement1'.
  *
  * @param wildcard A wildcard pattern to match against following statements.
  * @param statement1 The statement number of the potential followed statement.
  * @return true if matching statements are followed by 'statement1', false otherwise.
  */
  bool isFollow(Wildcard wildcard, statementNumber statement1);

  /**
  * @brief Checks if statements matching two wildcard patterns are in a follows relationship with each other.
  *
  * This function checks whether statements that match the given wildcard patterns are in a follows relationship with each other.
  *
  * @param wildcard A wildcard pattern to match against following statements.
  * @param wildcard2 A wildcard pattern to match against followed statements.
  * @return true if matching statements are in a follows relationship with each other, false otherwise.
  */
  bool isFollow(Wildcard wildcard, Wildcard wildcard2);

  /**
  * @brief Retrieves a set of statement numbers that are in a follow* relationship with statements matching a wildcard pattern of a specific type.
  *
  * This function returns an unordered set of statement numbers that are in a follow* relationship with statements of the specified type matching the given wildcard pattern.
  *
  * @param wildcard A wildcard pattern to match against followed statements.
  * @param entity The type of statement for which follow* relationships are sought.
  * @return An unordered set of statement numbers that are in a follow* relationship with matching statements of the specified type.
  */
  std::unordered_set<statementNumber> followStar(Wildcard wildcard, StmtEntity entity);

  /**
  * @brief Retrieves a set of statement numbers that are in a follow* relationship with a specified statement of a specific type.
  *
  * This function returns an unordered set of statement numbers that are in a follow* relationship with the specified statement and are of the specified type.
  *
  * @param num The statement number to which follow* relationships are sought.
  * @param entity The type of statement for which follow* relationships are sought.
  * @return An unordered set of statement numbers that are in a follow* relationship with the specified statement and match the specified type.
  */
  std::unordered_set<statementNumber> followStar(statementNumber num, StmtEntity entity);

  /**
  * @brief Retrieves a set of statement numbers that are in a follow* relationship with statements of a specific type, matching a wildcard pattern.
  *
  * This function returns an unordered set of statement numbers that are in a follow* relationship with statements of the specified type and match the given wildcard pattern.
  *
  * @param entity The type of statement for which follow* relationships are sought.
  * @param num The statement number to which follow* relationships are sought.
  * @return An unordered set of statement numbers that are in a follow* relationship with statements of the specified type.
  */
  std::unordered_set<statementNumber> followStar(StmtEntity entity, statementNumber num);

  /**
  * @brief Retrieves a set of statement numbers that are in a follow* relationship with statements matching a wildcard pattern of a specific type.
  *
  * This function returns an unordered set of statement numbers that are in a follow* relationship with statements of the specified type matching the given wildcard pattern.
  *
  * @param entity The type of statement for which follow* relationships are sought.
  * @param wildcard A wildcard pattern to match against followed statements.
  * @return An unordered set of statement numbers that are in a follow* relationship with matching statements of the specified type.
  */
  std::unordered_set<statementNumber> followStar(StmtEntity entity, Wildcard wildcard);

  /**
  * @brief Retrieves pairs of statement numbers that represent follow* relationships between statements of two specific types.
  *
  * This function returns an unordered set of pairs, where each pair consists of a statement number of the first type (entity1) and a statement number of the second type (entity2) that are in a follow* relationship.
  *
  * @param entity1 The type of the first statement in the follow* relationship.
  * @param entity2 The type of the second statement in the follow* relationship.
  * @return An unordered set of pairs representing follow* relationships between statements of the specified types.
  */
  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
  followStar(StmtEntity entity1, StmtEntity entity2);

  /**
  * @brief Checks if one statement is in a follow* relationship with another statement.
  *
  * This function checks whether the statement specified as 'statement1' is in a follow* relationship with the statement specified as 'statement2'.
  *
  * @param statement1 The statement number of the potential following statement.
  * @param statement2 The statement number of the potential followed statement.
  * @return true if 'statement1' is in a follow* relationship with 'statement2', false otherwise.
  */
  bool isFollowStar(statementNumber statement1, statementNumber statement2);

  /**
  * @brief Checks if one statement is in a follow* relationship with statements matching a wildcard pattern.
  *
  * This function checks whether the statement specified as 'statement1' is in a follow* relationship with statements that match the given wildcard pattern.
  *
  * @param statement1 The statement number of the potential following statement.
  * @param wildcard A wildcard pattern to match against followed statements.
  * @return true if 'statement1' is in a follow* relationship with matching statements, false otherwise.
  */
  bool isFollowStar(statementNumber statement1, Wildcard wildcard);

  /**
  * @brief Checks if statements matching a wildcard pattern are in a follow* relationship with a specified statement.
  *
  * This function checks whether statements that match the given wildcard pattern are in a follow* relationship with the statement specified as 'statement1'.
  *
  * @param wildcard A wildcard pattern to match against following statements.
  * @param statement1 The statement number of the potential followed statement.
  * @return true if matching statements are in a follow* relationship with 'statement1', false otherwise.
  */
  bool isFollowStar(Wildcard wildcard, statementNumber statement1);

  /**
  * @brief Checks if statements matching two wildcard patterns are in a follow* relationship with each other.
  *
  * This function checks whether statements that match the given wildcard patterns are in a follow* relationship with each other.
  *
  * @param wildcard A wildcard pattern to match against following statements.
  * @param wildcard2 A wildcard pattern to match against followed statements.
  * @return true if matching statements are in a follow* relationship with each other, false otherwise.
  */
  bool isFollowStar(Wildcard wildcard1, Wildcard wildcard2);

  /**
 * @brief Retrieves procedures that call a procedure matching a Wildcard.
 *
 * This function returns procedures that call a procedure matching the provided Wildcard.
 *
 * @param procedure The procedure to match against Wildcard.
 * @param wildcard The Wildcard used to match calling procedures.
 * @return A set of procedures calling procedures matching the Wildcard.
 */
  std::unordered_set<procedure> call(StmtEntity procedure, Wildcard wildcard);

/**
 * @brief Retrieves procedures that match a Wildcard and call a specific procedure.
 *
 * This function returns procedures matching a Wildcard that call the provided procedure.
 *
 * @param wildcard The Wildcard used to match against called procedures.
 * @param procedure The procedure to check for usage by procedures matching the Wildcard.
 * @return A set of procedures called by the provided procedure and matching the Wildcard.
 */
  std::unordered_set<procedure> call(Wildcard wildcard, StmtEntity procedure);

/** 
 * @brief Retrieves procedures that call a specific procedure.
 *
 * This function returns procedures that call the provided specific procedure.
 *
 * @param procedure The procedure being called.
 * @param proc The calling procedure.
 * @return A set of procedures calling the specified procedure.
 */
  std::unordered_set<procedure> call(procedure procedure, StmtEntity proc);

/**
 * @brief Retrieves procedures matching a Wildcard that call a specific procedure.
 *
 * This function returns procedures matching a Wildcard that call the provided specific procedure.
 *
 * @param proc The calling procedure.
 * @param procedure The procedure being called.
 * @return A set of procedures matching the Wildcard that call the specified procedure.
 */
  std::unordered_set<procedure> call(StmtEntity proc, procedure procedure);

/**
 * @brief Checks if a specific procedure calls a procedure matching a Wildcard.
 *
 * This function checks if the specified procedure calls a procedure matching the provided Wildcard.
 *
 * @param procedure The specific procedure being called.
 * @param wildcard The Wildcard used to match calling procedures.
 * @return True if the specified procedure calls procedures matching the Wildcard; false otherwise.
 */
  bool isCall(procedure procedure, Wildcard wildcard);

/**
 * @brief Checks if procedures matching a Wildcard call a specific procedure.
 *
 * This function checks if procedures matching the provided Wildcard call the specified procedure.
 *
 * @param wildcard The Wildcard used to match calling procedures.
 * @param procedure The specific procedure being called.
 * @return True if procedures matching the Wildcard call the specified procedure; false otherwise.
 */
  bool isCall(Wildcard wildcard, procedure procedure);

  /**
  * @brief Retrieves all pairs of procedures representing the Call relationship between two specified procedures.
  *
  * This function returns all pairs of procedures representing the Call relationship between two specified procedures.
  *
  * @param procedure1 The calling procedure.
  * @param procedure2 The called procedure.
  * @return A set of pairs, where each pair represents a procedure that directly calls another procedure (procedure1, procedure2).
  */
  std::unordered_set<std::pair<procedure, procedure>, PairHash>
  call(StmtEntity procedure1, StmtEntity procedure2);

  /**
* @brief Checks if a specific procedure calls another specific procedure.
*
* This function checks if a specified procedure calls another specified procedure.
*
* @param procedure1 The calling procedure.
* @param procedure2 The called procedure.
* @return True if the calling procedure calls the called procedure; false otherwise.
*/
  bool isCall(procedure procedure1, procedure procedure2);

/**
 * @brief Checks if there is a Call relationship between two procedures matching Wildcards.
 *
 * This function checks if there is a Call relationship between two procedures that match the provided Wildcards.
 *
 * @param wildcard The Wildcard for the calling procedure.
 * @param wildcard2 The Wildcard for the called procedure.
 * @return True if a Call relationship exists; false otherwise.
 */
  bool isCall(Wildcard wildcard, Wildcard wildcard2);

  /**
* @brief Retrieves all pairs of procedures representing the Call* (transitive call) relationship between two specified procedures.
*
* This function returns all pairs of procedures representing the transitive Call* relationship between two specified procedures.
*
* @param procedure1 The calling procedure.
* @param procedure2 The called procedure.
* @return A set of pairs, where each pair represents a procedure that directly or indirectly
*         calls another procedure (procedure1, procedure2).
*/
  std::unordered_set<std::pair<procedure, procedure>, PairHash>
  callStar(StmtEntity procedure1, StmtEntity procedure2);

/**
 * @brief Retrieves procedures that have a Call* (transitive call) relationship with a procedure matching a Wildcard.
 *
 * This function returns procedures that have a transitive Call* relationship with a procedure matching the provided Wildcard.
 *
 * @param procedure The procedure to match against Wildcard.
 * @param wildcard The Wildcard used to match procedures with transitive call relationships.
 * @return A set of procedures that have a Call* relationship with a procedure matching the Wildcard.
 */
  std::unordered_set<procedure> callStar(StmtEntity procedure, Wildcard wildcard);

/**
 * @brief Retrieves procedures that match a Wildcard and have a Call* (transitive call) relationship with a specific procedure.
 *
 * This function returns procedures matching a Wildcard that have a transitive Call* relationship with the provided procedure.
 *
 * @param wildcard The Wildcard used to match against procedures with transitive call relationships.
 * @param procedure The procedure with which procedures are checked for Call* relationships.
 * @return A set of procedures matching the Wildcard that have a Call* relationship with the provided procedure.
 */
  std::unordered_set<procedure> callStar(Wildcard wildcard, StmtEntity procedure);

/**
 * @brief Retrieves procedures that have a Call* (transitive call) relationship with a specific procedure.
 *
 * This function returns procedures that have a transitive Call* relationship with the provided specific procedure.
 *
 * @param procedure The specific procedure being called.
 * @param proc The calling procedure.
 * @return A set of procedures having a Call* relationship with the specified procedure.
 */
  std::unordered_set<procedure> callStar(procedure procedure, StmtEntity proc);

/**
 * @brief Retrieves procedures matching a Wildcard that have a Call* (transitive call) relationship with a specific procedure.
 *
 * This function returns procedures matching a Wildcard that have a transitive Call* relationship with the provided specific procedure.
 *
 * @param proc The specific procedure being called.
 * @param procedure The calling procedure.
 * @return A set of procedures matching the Wildcard having a Call* relationship with the specified procedure.
 */
  std::unordered_set<procedure> callStar(StmtEntity proc, procedure procedure);

/**
 * @brief Checks if a specific procedure has a Call* (transitive call) relationship with procedures matching a Wildcard.
 *
 * This function checks if the specified procedure has a transitive Call* relationship with procedures matching the provided Wildcard.
 *
 * @param procedure The specific procedure being called.
 * @param wildcard The Wildcard used to match calling procedures.
 * @return True if the specified procedure has a Call* relationship with procedures matching the Wildcard; false otherwise.
 */
  bool isCallStar(procedure procedure, Wildcard wildcard);

/**
 * @brief Checks if procedures matching a Wildcard have a Call* (transitive call) relationship with a specific procedure.
 *
 * This function checks if procedures matching the provided Wildcard have a transitive Call* relationship with the specified procedure.
 *
 * @param wildcard The Wildcard used to match calling procedures.
 * @param procedure The specific procedure being called.
 * @return True if procedures matching the Wildcard have a Call* relationship with the specified procedure; false otherwise.
 */
  bool isCallStar(Wildcard wildcard, procedure procedure);

/**
 * @brief Checks if a specific procedure has a Call* (transitive call) relationship with another specific procedure.
 *
 * This function checks if a specific procedure has a transitive Call* relationship with another specific procedure.
 *
 * @param procedure1 The calling procedure.
 * @param procedure2 The called procedure.
 * @return True if the calling procedure has a Call* relationship with the called procedure; false otherwise.
 */
  bool isCallStar(procedure procedure1, procedure procedure2);

/**
 * @brief Checks if there is a Call* (transitive call) relationship between two procedures matching Wildcards.
 *
 * This function checks if there is a Call* relationship between two procedures that match the provided Wildcards.
 *
 * @param wildcard1 The Wildcard for the calling procedure.
 * @param wildcard2 The Wildcard for the called procedure.
 * @return True if a Call* relationship exists; false otherwise.
 */
  bool isCallStar(Wildcard wildcard1, Wildcard wildcard2);

  /**
* @brief Retrieves a set of statement numbers associated with IF statements matching the given Wildcard.
*
* This function returns a set of statement numbers corresponding to IF statements that match the provided Wildcard.
*
* @param wc The Wildcard to match IF statements against.
* @return A set of statement numbers corresponding to IF statements matching the Wildcard.
*/
  std::unordered_set<statementNumber> getIf(Wildcard wc);

/**
 * @brief Retrieves a set of statement numbers associated with IF statements containing the specified variable.
 *
 * This function returns a set of statement numbers corresponding to IF statements that contain the provided variable.
 *
 * @param v The variable to match against IF statements.
 * @return A set of statement numbers corresponding to IF statements containing the variable.
 */
  std::unordered_set<statementNumber> getIf(variable v);

/**
 * @brief Retrieves all IF statements, represented as pairs of statementNumber and variable, from the data.
 *
 * This function retrieves all IF statements from the data and returns them as pairs of
 * statementNumber and variable.
 *
 * @return A set of pairs, where each pair represents an IF statement (statementNumber, variable).
 */
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAllIf();

/**
 * @brief Retrieves a set of statement numbers associated with WHILE statements matching the given Wildcard.
 *
 * This function returns a set of statement numbers corresponding to WHILE statements that match the provided Wildcard.
 *
 * @param wc The Wildcard to match WHILE statements against.
 * @return A set of statement numbers corresponding to WHILE statements matching the Wildcard.
 */
  std::unordered_set<statementNumber> getWhile(Wildcard wc);

/**
 * @brief Retrieves a set of statement numbers associated with WHILE statements containing the specified variable.
 *
 * This function returns a set of statement numbers corresponding to WHILE statements that contain the provided variable.
 *
 * @param v The variable to match against WHILE statements.
 * @return A set of statement numbers corresponding to WHILE statements containing the variable.
 */
  std::unordered_set<statementNumber> getWhile(variable v);

/**
 * @brief Retrieves all WHILE statements, represented as pairs of statementNumber and variable, from the data.
 *
 * This function retrieves all WHILE statements from the data and returns them as pairs of
 * statementNumber and variable.
 *
 * @return A set of pairs, where each pair represents a WHILE statement (statementNumber, variable).
 */
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAllWhile();

  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> Next(StmtEntity ent1, StmtEntity ent2);

  std::unordered_set<statementNumber> Next(StmtEntity ent, Wildcard);

  std::unordered_set<statementNumber> Next(StmtEntity ent, statementNumber num);

  std::unordered_set<statementNumber> Next(Wildcard, StmtEntity ent);

  std::unordered_set<statementNumber> Next(statementNumber num, StmtEntity ent);

  bool isNext(Wildcard, Wildcard);

  bool isNext(Wildcard, statementNumber num);

  bool isNext(statementNumber num, Wildcard);

  bool isNext(statementNumber num1, statementNumber num2);

  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> NextStar(StmtEntity ent1, StmtEntity ent2);

  std::unordered_set<statementNumber> NextStar(StmtEntity ent, Wildcard);

  std::unordered_set<statementNumber> NextStar(StmtEntity ent, statementNumber num);

  std::unordered_set<statementNumber> NextStar(Wildcard, StmtEntity ent);

  std::unordered_set<statementNumber> NextStar(statementNumber num, StmtEntity ent);

  bool isNextStar(Wildcard, Wildcard);

  bool isNextStar(Wildcard, statementNumber num);

  bool isNextStar(statementNumber num, Wildcard);

  bool isNextStar(statementNumber num1, statementNumber num2);

  std::unordered_set<std::pair<statementNumber, std::string>, PairHash> getStatementsAndVariable(StmtEntity type);

  bool isAffects(statementNumber statement1, statementNumber statement2);

  bool isAffects(statementNumber statement1, Wildcard w);

  bool isAffects(Wildcard w, statementNumber statement2);

  bool isAffects(Wildcard w, Wildcard w2);

  std::unordered_set<statementNumber> Affects(StmtEntity stmtEntity, Wildcard w);

  std::unordered_set<statementNumber> Affects(Wildcard w, StmtEntity stmtEntity);

  std::unordered_set<statementNumber> Affects(StmtEntity stmtEntity, statementNumber stmt);

  std::unordered_set<statementNumber> Affects(statementNumber stmt, StmtEntity stmtEntity);

  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> Affects();
};
