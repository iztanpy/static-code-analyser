#pragma once

// TODO(phuccuongngo99): Sort this as well
#include <string>
#include <variant>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <utility>

// TODO(phuccuongngo99): Learn how to define these type properly
using Header = std::string;
using Cell = std::string;
using Col = std::vector<Cell>;
using Table = std::unordered_map<Header, Col>;

// TODO(phuccuongngo99): Maybe better name here?
using Constraint = struct {
  std::pair<Header, Header> headers;
  std::unordered_set<std::pair<Cell, Cell>> valid_pairs;
};

class ConstraintTable {
  // TODO(phuccuongngo99): Make this private later, consider making other things private
  // too
 public:
  Table table;

  // TODO(phuccuongngo99): This is just for testing purposes
  explicit ConstraintTable(const Table& table);

  void Solve(Constraint& constraint);

  std::vector<std::string> Select(Header& header);

  // TODO(phuccuongngo99): Better documentation here
  // Add new header to the table
  void AddNewUnaryConstraint(const Header& new_header, const Col& new_values);

  void AddNewBinaryConstraint(const Header& new_header1, const Col& new_values1,
                              const Header& new_header2, const Col& new_values2);

  void AddExistingUnaryConstraint(const Header& existing_header, const Col& new_values);

  void AddExistingBinaryConstraint(const Header& existing_header1, const Col& new_values1,
                                   const Header& existing_header2, const Col& new_values2);
  void AddHalfExistingBinaryConstraint(const Header& existing_header, const Col& new_existing_values,
                                       const Header& new_header, const Col& new_values);
};
