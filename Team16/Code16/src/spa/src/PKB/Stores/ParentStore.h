#pragma once

#include <string>
#include <iostream>

#include <unordered_set>
#include <unordered_map>
#include "utils/entity_types.h"
#include "utils/clauses_types.h"

class ParentStore {
 private:
  typedef std::string variable;
  typedef int statementNumber;
  std::unordered_map<statementNumber, std::unordered_set<statementNumber>> ParentMap;
  std::unordered_map<statementNumber, statementNumber> ParentMapReverse;
  std::unordered_map<statementNumber, std::unordered_set<statementNumber>> ParentStarMap;
  std::unordered_map<statementNumber, std::unordered_set<statementNumber>> ParentStarMapReverse;

 public:
  ParentStore();

  /**
  * @brief Stores parent-child relationships between statements.
  *
  * This method stores parent-child relationships represented by an unordered map (map), where each statement number (key)
  * is associated with a set of statement numbers (values) that are its children.
  *
  * @param map An unordered map of statement numbers to sets of child statement numbers.
  */
  void storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map);

  /**
  * @brief Checks if a specific statement is a direct parent of another statement.
  *
  * This method checks if the statement specified by 'parent' is a direct parent of the statement specified by 'child'.
  *
  * @param parent The statement number to check as the parent statement.
  * @param child The statement number to check as the child statement.
  * @return true if 'parent' is a direct parent of 'child', false otherwise.
  */
  bool isParent(statementNumber parent, statementNumber child);

  /**
  * @brief Checks if a specific statement is a direct parent of statements matching a wildcard pattern.
  *
  * This method checks if the statement specified by 'parent' is a direct parent of statements matching the wildcard pattern (wildcard).
  *
  * @param parent The statement number to check as the parent statement.
  * @param wildcard A wildcard pattern for child statements.
  * @return true if 'parent' is a direct parent of matching child statements, false otherwise.
  */
  bool isParent(statementNumber parent, Wildcard wildcard);

  /**
  * @brief Checks if statements matching a wildcard pattern have a direct parent specified by its number.
  *
  * This method checks if statements matching the wildcard pattern (wildcard) have a direct parent specified by 'child'.
  *
  * @param wildcard A wildcard pattern for child statements.
  * @param child The statement number to check as the parent statement.
  * @return true if matching child statements have 'child' as a direct parent, false otherwise.
  */
  bool isParent(Wildcard wildcard, statementNumber child);

  /**
  * @brief Checks if statements matching a wildcard pattern have a direct parent matching another wildcard pattern.
  *
  * This method checks if statements matching the first wildcard pattern (wildcard) have a direct parent matching
  * the second wildcard pattern (wildcard2).
  *
  * @param wildcard A wildcard pattern for child statements.
  * @param wildcard2 A wildcard pattern for parent statements.
  * @return true if matching child statements have matching parent statements as direct parents, false otherwise.
  */
  bool isParent(Wildcard wildcard, Wildcard wildcard2);

  /**
  * @brief Retrieves the children of a specific statement.
  *
  * This method returns an unordered set containing the statement numbers that are direct children of the specified statement (statement).
  *
  * @param statement The statement number for which to retrieve children.
  * @return An unordered set of statement numbers representing direct children of the specified statement.
  */
  std::unordered_set<statementNumber> getChildren(statementNumber statement);

  /**
  * @brief Retrieves the parent(s) of a specific statement.
  *
  * This method returns an unordered set containing the statement numbers that are direct parents of the specified statement (statement).
  *
  * @param statement The statement number for which to retrieve parent(s).
  * @return An unordered set of statement numbers representing direct parent(s) of the specified statement.
  */
  std::unordered_set<statementNumber> getParent(statementNumber statement);

  /**
  * @brief Checks if a specific statement is a parent of another statement in the parent-star relationship.
  *
  * This method checks if the statement specified by 'parent' is a parent of the statement specified by 'child'
  * in the parent-star relationship, considering all possible transitive parent relationships.
  *
  * @param parent The statement number to check as the parent statement.
  * @param child The statement number to check as the child statement.
  * @return true if 'parent' is a parent of 'child' in the parent-star relationship, false otherwise.
  */
  bool isParentStar(statementNumber parent, statementNumber child);

  /**
  * @brief Checks if a specific statement is a parent of statements matching a wildcard pattern in the parent-star relationship.
  *
  * This method checks if the statement specified by 'parent' is a parent of statements matching the wildcard pattern (wildcard)
  * in the parent-star relationship, considering all possible transitive parent relationships.
  *
  * @param parent The statement number to check as the parent statement.
  * @param wildcard A wildcard pattern for child statements.
  * @return true if 'parent' is a parent of matching child statements in the parent-star relationship, false otherwise.
  */
  bool isParentStar(statementNumber parent, Wildcard wildcard);

  /**
  * @brief Checks if statements matching a wildcard pattern have a parent specified by its number in the parent-star relationship.
  *
  * This method checks if statements matching the wildcard pattern (wildcard) have a parent specified by 'child'
  * in the parent-star relationship, considering all possible transitive parent relationships.
  *
  * @param wildcard A wildcard pattern for child statements.
  * @param child The statement number to check as the parent statement.
  * @return true if matching child statements have 'child' as a parent in the parent-star relationship, false otherwise.
  */
  bool isParentStar(Wildcard wildcard, statementNumber child);

  /**
  * @brief Checks if statements matching a wildcard pattern have a parent matching another wildcard pattern in the parent-star relationship.
  *
  * This method checks if statements matching the first wildcard pattern (wildcard) have a parent matching the second wildcard pattern (wildcard2)
  * in the parent-star relationship, considering all possible transitive parent relationships.
  *
  * @param wildcard A wildcard pattern for child statements.
  * @param wildcard2 A wildcard pattern for parent statements.
  * @return true if matching child statements have matching parent statements as parents in the parent-star relationship, false otherwise.
  */
  bool isParentStar(Wildcard wildcard, Wildcard wildcard2);

  /**
  * @brief Retrieves the direct children of a specific statement.
  *
  * This method returns an unordered set containing the statement numbers that are direct children of the specified statement (statement).
  *
  * @param statement The statement number for which to retrieve direct children.
  * @return An unordered set of statement numbers representing direct children of the specified statement.
  */
  std::unordered_set<statementNumber> getChildrens(statementNumber statement);

  /**
  * @brief Retrieves the direct parent(s) of a specific statement.
  *
  * This method returns an unordered set containing the statement numbers that are direct parent(s) of the specified statement (statement).
  *
  * @param statement The statement number for which to retrieve direct parent(s).
  * @return An unordered set of statement numbers representing direct parent(s) of the specified statement.
  */
  std::unordered_set<statementNumber> getParents(statementNumber statement);
};
