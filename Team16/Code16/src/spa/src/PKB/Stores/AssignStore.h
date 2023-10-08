#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "utils/clauses_types.h"
#include "utils/entity_types.h"
#include "utils/hash_utils.h"

class AssignStore {
 private:
  typedef std::string variable;
  typedef std::string full;
  typedef std::string partialMatch;
  typedef int statementNumber;
  std::unordered_map<statementNumber, std::unordered_set<partialMatch>> numRHSMap;
  std::unordered_map<statementNumber, variable> numLHSMap;
  std::unordered_map<variable, std::unordered_set<statementNumber>> reverseNumLHSMap;
  std::unordered_map<partialMatch, std::unordered_set<statementNumber>> reverseNumRHSMap;

  std::unordered_map<statementNumber, full> fullRHSMap;
  std::unordered_map<full, std::unordered_set<statementNumber>> reverseFullRHSMap;

 public:
  AssignStore();

  void storeFullPatternAssign(std::unordered_map<statementNumber, full> relations);

  std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPairPartial(partialMatch partial);
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPairFull(full full);
  std::unordered_set<statementNumber> getAssignsWcF(Wildcard lhs, full rhs);
  std::unordered_set<statementNumber> getAssignsFF(full lhs, full rhs);



    /**
    * @brief Adds a mapping of statement numbers to partial match sets for right-hand sides (RHS) of assignments.
    *
    * This method adds a mapping of statement numbers to sets of partial matches representing the right-hand sides (RHS)
    * of assignments. The mapping allows for efficient retrieval of RHS information for assignments.
    *
    * @param numRHSMap An unordered map of statement numbers to sets of partial matches for RHS.
    */
  void addNumRHSMap(std::unordered_map<statementNumber, std::unordered_set<partialMatch>> numRHSMap);

  /**
  * @brief Adds a mapping of statement numbers to variables for left-hand sides (LHS) of assignments.
  *
  * This method adds a mapping of statement numbers to variables representing the left-hand sides (LHS) of assignments.
  * The mapping allows for efficient retrieval of LHS information for assignments.
  *
  * @param numLHSMap An unordered map of statement numbers to variables for LHS.
  */
  void addNumLHSMap(std::unordered_map<statementNumber, variable> numLHSMap);

  /**
  * @brief Retrieves all statement numbers corresponding to assignment statements.
  *
  * This method returns an unordered set containing all statement numbers that correspond to assignment statements.
  *
  * @return An unordered set of statement numbers corresponding to assignment statements.
  */
  std::unordered_set<statementNumber> getAllAssigns();

  /**
  * @brief Retrieves assignment statements matching a specific partial right-hand side (RHS) pattern.
  *
  * This method returns an unordered set of pairs, where each pair consists of a statement number and a variable,
  * representing assignment statements that match the provided partial right-hand side (RHS) pattern (partial).
  *
  * @param partial The partial match pattern for RHS of assignments.
  * @return An unordered set of pairs representing assignment statements matching the partial RHS pattern.
  */
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPair(partialMatch partial);

  /**
  * @brief Retrieves assignment statements matching a specific wildcard right-hand side (RHS) pattern.
  *
  * This method returns an unordered set of pairs, where each pair consists of a statement number and a variable,
  * representing assignment statements that match the provided wildcard right-hand side (RHS) pattern (wildcard).
  *
  * @param wildcard The wildcard pattern for RHS of assignments.
  * @return An unordered set of pairs representing assignment statements matching the wildcard RHS pattern.
  */
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPair(Wildcard wildcard);

  /**
  * @brief Retrieves assignment statements matching specific left-hand side (LHS) and partial right-hand side (RHS) patterns.
  *
  * This method returns an unordered set of statement numbers representing assignment statements that match
  * the provided left-hand side (LHS) and partial right-hand side (RHS) patterns.
  *
  * @param lhs The wildcard pattern for LHS of assignments.
  * @param rhs The partial match pattern for RHS of assignments.
  * @return An unordered set of statement numbers representing assignment statements matching the specified patterns.
  */
  std::unordered_set<statementNumber> getAssigns(Wildcard lhs, partialMatch rhs);

  /**
  * @brief Retrieves assignment statements matching specific wildcard left-hand side (LHS) and wildcard right-hand side (RHS) patterns.
  *
  * This method returns an unordered set of statement numbers representing assignment statements that match
  * the provided wildcard left-hand side (LHS) and wildcard right-hand side (RHS) patterns.
  *
  * @param lhs The wildcard pattern for LHS of assignments.
  * @param rhs The wildcard pattern for RHS of assignments.
  * @return An unordered set of statement numbers representing assignment statements matching the specified patterns.
  */
  std::unordered_set<statementNumber> getAssigns(Wildcard lhs, Wildcard rhs);

  /**
  * @brief Retrieves assignment statements matching specific partial left-hand side (LHS) and partial right-hand side (RHS) patterns.
  *
  * This method returns an unordered set of statement numbers representing assignment statements that match
  * the provided partial left-hand side (LHS) and partial right-hand side (RHS) patterns.
  *
  * @param lhs The partial match pattern for LHS of assignments.
  * @param rhs The partial match pattern for RHS of assignments.
  * @return An unordered set of statement numbers representing assignment statements matching the specified patterns.
  */
  std::unordered_set<statementNumber> getAssigns(partialMatch lhs, partialMatch rhs);

  /**
  * @brief Retrieves assignment statements matching specific partial left-hand side (LHS) and wildcard right-hand side (RHS) patterns.
  *
  * This method returns an unordered set of statement numbers representing assignment statements that match
  * the provided partial left-hand side (LHS) and wildcard right-hand side (RHS) patterns.
  *
  * @param lhs The partial match pattern for LHS of assignments.
  * @param rhs The wildcard pattern for RHS of assignments.
  * @return An unordered set of statement numbers representing assignment statements matching the specified patterns.
  */
  std::unordered_set<statementNumber> getAssigns(partialMatch lhs, Wildcard rhs);
};
