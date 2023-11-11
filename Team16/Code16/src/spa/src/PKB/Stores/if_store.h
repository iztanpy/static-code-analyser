#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "utils/clauses_types.h"
#include "utils/entity_types.h"
#include "utils/hash_utils.h"

class if_store {
 private:
  typedef std::string variable;
  typedef int statementNumber;
  std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap;
  std::unordered_map<variable, std::unordered_set<statementNumber>> reverseVariableMap;

 public:
  if_store();
  /**
  * @brief Adds a mapping of statement numbers to sets of variables for use in IF statements.
  *
  * This function allows you to add a mapping of statement numbers to sets of variables for use in IF statements.
  *
  * @param variableMap An unordered_map where statementNumber is the key, and unordered_set<variable> is the associated value.
  */
  void addVariableMap(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap);

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
  * This function retrieves all IF statements from the data and returns them as pairs of statementNumber and variable.
  *
  * @return A set of pairs, where each pair represents an IF statement (statementNumber, variable).
  */
  std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAllIf();
};

