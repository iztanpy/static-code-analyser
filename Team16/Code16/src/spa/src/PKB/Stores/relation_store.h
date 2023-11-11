#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "utils/hash_utils.h"

class relation_store {
 private:
  typedef std::string variable;
  typedef std::string procedure;

  typedef int statementNumber;
  std::unordered_map<statementNumber, std::unordered_set<variable>> ForwardVariableStore;
  std::unordered_map<variable, std::unordered_set<statementNumber>> ReverseVariableStore;

  std::unordered_map<procedure, std::unordered_set<variable>> ForwardProcedureStore;
  std::unordered_map<variable, std::unordered_set<procedure>> ReverseProcedureStore;

 public:
  relation_store();

  /**
  * @brief Stores unspecified relation relationships between statements and variables.
  *
  * This method stores relationships represented by an unordered map (relations), where each statement number (key)
  * is associated with a set of variables (values) that are modified by that statement.
  *
  * @param relations An unordered map of statement numbers to sets of variables modified by each statement.
  */
  void storeRelation(std::unordered_map<statementNumber, std::unordered_set<variable>> relations);

  void storeRelationProcedures(std::unordered_map<procedure,
                                                  std::pair<int, int>> procedures,
                               std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar);

  void storeRelationCalls(std::unordered_map<statementNumber, procedure> calls,
                          std::unordered_map<statementNumber, std::unordered_set<statementNumber>> callsParentMap);

  /**
  * @brief Checks if a specific statement has a relation to a particular variable.
  *
  * This method checks if the statement specified by 'statement' has a relation to the variable specified by 'variable'.
  *
  * @param statement The statement number to check for variable relation.
  * @param variable The variable to check for having a relation by the statement.
  * @return true if the statement applies the relation on the variable, false otherwise.
  */
  bool isRelation(statementNumber statement, variable variable);

  /**
  * @brief Checks if a specific statement has a relation to any variable.
  *
  * This method checks if the statement specified by 'statement' relates any variables.
  *
  * @param statement The statement number to check for variable relation.
  * @return true if the statement relates to at least one variable, false otherwise.
  */
  bool isRelation(statementNumber statement);

  /**
  * @brief Retrieves the variables related by a specific statement.
  *
  * This method returns an unordered set containing the variables that are related by the statement specified by 'statement'.
  *
  * @param statement The statement number for which to retrieve related variables.
  * @return An unordered set of variables related to by the specified statement.
  */
  std::unordered_set<variable> relates(statementNumber statement);

  bool isRelation(procedure procedure);

  std::unordered_set<variable> relatesProcedureProc(procedure procedure);

  bool isRelation(procedure procedure, variable variable);

  std::unordered_set<procedure> relatesProcedure();

  std::unordered_set<procedure> relatesProcedure(variable variable);

  std::unordered_set<std::pair<procedure, variable>, PairHash> relatesProcedurePair();

  /**
  * @brief Retrieves the statements that modify a particular variable.
  *
  * This method returns an unordered set of statement numbers representing statements that modify the variable specified by 'variable'.
  *
  * @param variable The variable for which to find modifying statements.
  * @return An unordered set of statement numbers of statements that modify the specified variable.
  */
  std::unordered_set<statementNumber> relates(variable variable);
};
