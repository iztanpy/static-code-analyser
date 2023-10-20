#pragma once

#include <string>
#include <variant>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <utility>
#include <sstream>

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "qps/query_evaluator/constraint_solver/constraint_table.h"

class ConstraintTable {
 public:
  ConstraintTable();

  void JoinTable(const ConstraintTable& constraint_table);

  void Filter(const std::vector<ColName>& col_names);

  /*!
   * @brief Adds a new constraint to the constraint table.
   *
   * This method adds a new constraint to the constraint table. The constraint can be a unary constraint, a binary
   * constraint, or a boolean constraint.
   *
   * @param constraint The constraint to be added to the constraint table.
   */
  void Solve(Constraint& constraint);

  /*!
  // To check if Constraints supplied so far are valid
  // With valid being: No False Constraint, or All True Constraint
  // Or Unary/Binary Constraint result in at least 1 valid assignments
  // for all synonym
   */
  bool IsValid();

  /*!
   * @brief Retrieves all values of a specified column from the constraint table.
   * Header field is the first column of the table, else assert will fail
   * @param col_name
   * @return A set of all values of the specified column.
   */
  std::unordered_set<std::string> Select(const std::vector<ColName>& selects);

  /*!
   * @return all availables column names in the constraint table
   */
  std::unordered_set<ColName> AvailableColName();

  /*!
   * @return Table for unit test or integration test, not to be used in main code
   */
  Table GetTableForTesting();

  /*!
   * @param mock_table
   * @return ConstraintTable for unit test or integration test, not to be used in main code
   */
  static ConstraintTable ForTestingOnly(const Table& mock_table) {
    return ConstraintTable(mock_table);
  }

 private:
  Table table;

  /*!
   * Check if ConstraintTable has been given a Constraint that evaluates
   * to false before. This is useful for HasNoValidValues() function
   */
  bool has_at_least_one_false_constraint;

  /*!
   * Check if ConstraintTable has only been given True Constraint
   */
  bool has_only_true_constraint;

  /*!
   * @brief Constructor for ConstraintTable for unit test or integration test, not to be used in main code
   * @param mock_table
   */
  explicit ConstraintTable(const Table& mock_table) : table(mock_table) {}

  /*!
   * @brief Adds a new unary constraint to the constraint table.
   * @param constraint
   */
  void Solve(const UnaryConstraint& constraint);

  /*!
   * @brief Adds a new binary constraint to the constraint table.
   * @param constraint
   */
  void Solve(const BinaryConstraint& constraint);

  /*!
   * @brief Adds a new boolean constraint to the constraint table.
   * @param constraint
   */
  void Solve(const bool constraint);

  /*!
   * @brief Adds a new unary constraint to the constraint table.
   * @param new_constraint
   */
  void AddNewUnaryConstraint(const UnaryConstraint& new_constraint);

  /*!
   * @brief Adds a new binary constraint to the constraint table.
   * @param new_constraint
   */
  void AddNewBinaryConstraint(const BinaryConstraint& new_constraint);

  /*!
   * @brief Adds a existing unary constraint to the constraint table.
   * @param existing_constraint
   */
  void AddExistingUnaryConstraint(const UnaryConstraint& existing_constraint);

  /*!
   * @brief Adds a existing binary constraint to the constraint table.
   * @param existing_constraint
   */
  void AddExistingBinaryConstraint(const BinaryConstraint& existing_constraint);

  /*!
   * @brief Adds a binary constraint with 1 new column name and 1 existing column name
   * @param constraint
   */
  void AddHalfExistingBinaryConstraint(const BinaryConstraint& constraint);
};
