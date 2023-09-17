#pragma once

// TODO(phuccuongngo99): Sort this as well and remove unnecessary ones
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
  ConstraintTable() = default;

  void Solve(Constraint& constraint);

  // TODO(phuccuongngo99): As of now, header field must exist within
  std::unordered_set<std::string> Select(const ColName& col_name);

  std::unordered_set<ColName> AvailableColName();

  Table GetTableForTesting();

  // For testing only
  static ConstraintTable ForTestingOnly(const Table& mock_table) {
    return ConstraintTable(mock_table);
  }

 private:
  Table table;

  // Mock table for unit test
  explicit ConstraintTable(const Table& mock_table) : table(mock_table) {}

  void Solve(const UnaryConstraint& constraint);
  void Solve(const BinaryConstraint& constraint);

  void AddNewUnaryConstraint(const UnaryConstraint& new_constraint);

  void AddNewBinaryConstraint(const BinaryConstraint& new_constraint);

  void AddExistingUnaryConstraint(const UnaryConstraint& existing_constraint);

  void AddExistingBinaryConstraint(const BinaryConstraint& existing_constraint);

  // When adding a binary constraint with 1 new column name and 1 existing column name
  void AddHalfExistingBinaryConstraint(const BinaryConstraint& constraint);
};
