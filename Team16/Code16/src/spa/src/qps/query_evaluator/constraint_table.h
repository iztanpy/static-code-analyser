#pragma once

#include <string>
#include <variant>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <utility>

#include "qps/query_evaluator/constraint.h"
#include "qps/query_evaluator/constraint_table.h"

class ConstraintTable {
 public:
  ConstraintTable();

  void Solve(Constraint& constraint);

  // To check if Constraints supplied so far are valid
  // With valid being: No False Constraint, or All True Constraint
  // Or Unary/Binary Constraint result in at least 1 valid assignments
  // for all synonym
  bool IsValid();

  // Header field is the first column of the table, else assert will fail
  std::unordered_set<std::string> Select(const ColName& col_name);

  std::unordered_set<ColName> AvailableColName();

  Table GetTableForTesting();

  // For testing only
  static ConstraintTable ForTestingOnly(const Table& mock_table) {
    return ConstraintTable(mock_table);
  }

 private:
  Table table;

  // Check if ConstraintTable has been given a Constraint that evaluates
  // to false before. This is useful for HasNoValidValues() function
  bool has_at_least_one_false_constraint;

  // Check if ConstraintTable has only been given True Constraint
  bool has_only_true_constraint;

  // Mock table for unit test
  explicit ConstraintTable(const Table& mock_table) : table(mock_table) {}

  void Solve(const UnaryConstraint& constraint);
  void Solve(const BinaryConstraint& constraint);
  void Solve(const bool constraint);

  void AddNewUnaryConstraint(const UnaryConstraint& new_constraint);

  void AddNewBinaryConstraint(const BinaryConstraint& new_constraint);

  void AddExistingUnaryConstraint(const UnaryConstraint& existing_constraint);

  void AddExistingBinaryConstraint(const BinaryConstraint& existing_constraint);

  // When adding a binary constraint with 1 new column name and 1 existing column name
  void AddHalfExistingBinaryConstraint(const BinaryConstraint& constraint);
};
