#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>

#include "utils/entity_types.h"
#include "utils/clauses_types.h"
#include "./StatementStore.h"

class FollowsStore {
 private:
  typedef int statementNumber;
  typedef std::string variable;
  typedef std::string stmtentity;
  std::unordered_map<statementNumber, statementNumber> FollowMap;
  std::unordered_map<statementNumber, statementNumber> FollowMapReverse;
  std::unordered_map<statementNumber, std::unordered_set<statementNumber>> FollowStarMap;
  std::unordered_map<statementNumber, std::unordered_set<statementNumber>> FollowStarMapReverse;

 public:
  FollowsStore();

  /**
  * @brief Stores follows relationships between statements.
  *
  * This method stores follows relationships represented by an unordered map (map), where each statement number (key)
  * is associated with the statement number (value) that directly follows it.
  *
  * @param map An unordered map of statement numbers to the statement numbers that follow each statement.
  */
  void storeFollows(std::unordered_map<statementNumber, statementNumber> map);


  // Follow methods

  /**
  * @brief Retrieves the statement that comes immediately after a specific statement.
  *
  * This method returns the statement number of the statement that comes immediately after the specified statement (statement).
  *
  * @param statement The statement number for which to find the statement that comes after.
  * @return The statement number that comes immediately after the specified statement, or an invalid statement number if there is none.
  */
  statementNumber getAfter(statementNumber statement);

  /**
  * @brief Checks if one statement follows another statement directly.
  *
  * This method checks if the statement specified by 'statement1' follows the statement specified by 'statement2' directly.
  *
  * @param statement1 The statement number to check as the following statement.
  * @param statement2 The statement number to check as the preceding statement.
  * @return true if 'statement1' follows 'statement2' directly, false otherwise.
  */
  bool isFollow(statementNumber statement1, statementNumber statement2);

  /**
  * @brief Checks if one statement follows another statement directly based on a wildcard pattern.
  *
  * This method checks if statements matching the wildcard pattern (wildcard) follow the statement specified by 'statement1' directly.
  *
  * @param statement1 The statement number to check as the preceding statement.
  * @param wildcard A wildcard pattern for following statements.
  * @return true if matching following statements follow 'statement1' directly, false otherwise.
  */
  bool isFollow(statementNumber statement1, Wildcard wildcard);

  /**
  * @brief Checks if statements matching a wildcard pattern follow a specific statement directly.
  *
  * This method checks if statements matching the wildcard pattern (wildcard) follow the statement specified by 'statement1' directly.
  *
  * @param wildcard A wildcard pattern for preceding statements.
  * @param statement1 The statement number to check as the following statement.
  * @return true if matching preceding statements follow 'statement1' directly, false otherwise.
  */
  bool isFollow(Wildcard wildcard, statementNumber statement1);

  /**
  * @brief Checks if statements matching a wildcard pattern follow statements matching another wildcard pattern directly.
  *
  * This method checks if statements matching the first wildcard pattern (wildcard) follow statements matching the second wildcard pattern (wildcard2) directly.
  *
  * @param wildcard A wildcard pattern for preceding statements.
  * @param wildcard2 A wildcard pattern for following statements.
  * @return true if matching preceding statements follow matching following statements directly, false otherwise.
  */
  bool isFollow(Wildcard wildcard, Wildcard wildcard2);

  /**
  * @brief Retrieves the statement that comes immediately before a specific statement.
  *
  * This method returns the statement number of the statement that comes immediately before the specified statement (statement).
  *
  * @param statement The statement number for which to find the statement that comes before.
  * @return The statement number that comes immediately before the specified statement, or an invalid statement number if there is none.
  */
  statementNumber getBefore(statementNumber statement);


  // Follow* methods

  /**
  * @brief Checks if one statement follows another statement in the follow-star relationship.
  *
  * This method checks if the statement specified by 'statement1' follows the statement specified by 'statement2'
  * in the follow-star relationship, considering all possible transitive follow relationships.
  *
  * @param statement1 The statement number to check as the following statement.
  * @param statement2 The statement number to check as the preceding statement.
  * @return true if 'statement1' follows 'statement2' in the follow-star relationship, false otherwise.
  */
  bool isFollowStar(statementNumber statement1, statementNumber statement2);

  /**
  * @brief Checks if one statement follows statements matching a wildcard pattern in the follow-star relationship.
  *
  * This method checks if the statement specified by 'statement1' follows statements matching the wildcard pattern (wildcard)
  * in the follow-star relationship, considering all possible transitive follow relationships.
  *
  * @param statement1 The statement number to check as the following statement.
  * @param wildcard A wildcard pattern for preceding statements.
  * @return true if 'statement1' follows matching preceding statements in the follow-star relationship, false otherwise.
  */
  bool isFollowStar(statementNumber statement1, Wildcard wildcard);

  /**
  * @brief Checks if statements matching a wildcard pattern follow a specific statement in the follow-star relationship.
  *
  * This method checks if statements matching the wildcard pattern (wildcard) follow the statement specified by 'statement1'
  * in the follow-star relationship, considering all possible transitive follow relationships.
  *
  * @param wildcard A wildcard pattern for following statements.
  * @param statement1 The statement number to check as the preceding statement.
  * @return true if matching following statements follow 'statement1' in the follow-star relationship, false otherwise.
  */
  bool isFollowStar(Wildcard wildcard, statementNumber statement1);

  /**
  * @brief Checks if statements matching a wildcard pattern follow statements matching another wildcard pattern in the follow-star relationship.
  *
  * This method checks if statements matching the first wildcard pattern (wildcard) follow statements matching the second wildcard pattern (wildcard2)
  * in the follow-star relationship, considering all possible transitive follow relationships.
  *
  * @param wildcard A wildcard pattern for preceding statements.
  * @param wildcard2 A wildcard pattern for following statements.
  * @return true if matching preceding statements follow matching following statements in the follow-star relationship, false otherwise.
  */
  bool isFollowStar(Wildcard wildcard, Wildcard wildcard2);

  /**
  * @brief Retrieves all statements that come immediately after a specific statement.
  *
  * This method returns an unordered set containing the statement numbers of all statements that come immediately after the specified statement (statement).
  *
  * @param statement The statement number for which to retrieve statements that come after.
  * @return An unordered set of statement numbers representing statements that come immediately after the specified statement.
  */
  std::unordered_set<statementNumber> getAfters(statementNumber statement);

  /**
  * @brief Retrieves all statements that come immediately before a specific statement.
  *
  * This method returns an unordered set containing the statement numbers of all statements that come immediately before the specified statement (statement).
  *
  * @param statement The statement number for which to retrieve statements that come before.
  * @return An unordered set of statement numbers representing statements that come immediately before the specified statement.
  */
  std::unordered_set<statementNumber> getBefores(statementNumber statement);
};
