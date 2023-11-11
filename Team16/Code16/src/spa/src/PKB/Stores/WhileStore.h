#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "utils/clauses_types.h"
#include "utils/entity_types.h"
#include "utils/hash_utils.h"

class WhileStore {
 private:
  typedef std::string variable;
  typedef int statementNumber;
  std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap;
  std::unordered_map<variable, std::unordered_set<statementNumber>> reverseVariableMap;

 public:
  WhileStore();
/**
 * Adds a mapping of statement numbers to sets of variables in the variableMap.
 *
 * @param variableMap A std::unordered_map where statementNumber is the key
 *                    and std::unordered_set<variable> is the associated value.
 */
  void addVariableMap(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap);

/**
 * Retrieves a set of statement numbers associated with while loops matching the given Wildcard.
 *
 * @param wc The Wildcard to match while loops against.
 * @return A set of statement numbers corresponding to while loops matching the Wildcard.
 */
  std::unordered_set<statementNumber> getWhile(Wildcard wc);

/**
 * Retrieves a set of statement numbers associated with while loops containing the specified variable.
 *
 * @param v The variable to match against while loops.
 * @return A set of statement numbers corresponding to while loops containing the variable.
 */
  std::unordered_set<statementNumber> getWhile(variable v);

/**
 * Retrieves all while loops, represented as pairs of statementNumber and variable, from the data.
 *
 * @return A set of pairs, where each pair represents a while loop (statementNumber, variable).
 */
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAllWhile();
};
