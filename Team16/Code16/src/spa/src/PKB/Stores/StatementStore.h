#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "utils/entity_types.h"

class StatementStore {
 private:
  typedef std::string variable;
  typedef std::string constant;
  typedef int statementNumber;

  std::unordered_map<statementNumber, StmtEntity> statementMap;
  std::unordered_map<StmtEntity, std::unordered_set<statementNumber>> statementMapReverse;

 public:
  StatementStore();

  /**
  * @brief Adds statements and their associated types to the statement store.
  *
  * This method adds statements represented by an unordered map (typeMap), where each statement number (key) is associated with
  * a statement type (value), to the statement store.
  *
  * @param typeMap An unordered map of statement numbers to their associated statement types.
  */
  void addStatements(std::unordered_map<statementNumber, StmtEntity> typeMap);

  /**
  * @brief Retrieves all statements of a specific statement type from the statement store.
  *
  * This method returns an unordered set containing all the statement numbers associated with the specified statement type (type).
  *
  * @param type The type of statements to retrieve.
  * @return An unordered set of statement numbers of the specified statement type.
  */
  std::unordered_set<statementNumber> getStatements(StmtEntity type);

  /**
  * @brief Retrieves all statements stored in the statement store, regardless of their types.
  *
  * This method returns an unordered set containing all the statement numbers stored in the statement store, regardless of their types.
  *
  * @return An unordered set of all statement numbers stored in the statement store.
  */
  std::unordered_set<statementNumber> getAllStatements();
};

