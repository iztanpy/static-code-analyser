#include "qps/query_evaluator/constraint_table.h"

// TODO(phuccuongngo99): Convert these function to template and throw them into Util
// {1, 2, 3}, 2 -> {1, 1, 2, 2, 3, 3}
std::vector<std::string> RepeatElements(const std::vector<std::string>& vec, int n) {
  std::vector<std::string> result;
  for (const std::string& val : vec) {
    for (int i = 0; i < n; ++i) {
      result.push_back(val);
    }
  }
  return result;
}
std::vector<std::string> RepeatVector(const std::vector<std::string>& vec, int n) {
  std::vector<std::string> result;
  for (int i = 0; i < n; ++i) {
    for (const std::string& val : vec) {
      result.push_back(val);
    }
  }
  return result;
}

template<typename T>
std::vector<T> ExtractFromIndices(const std::vector<T>& vec, const std::vector<int>& indices) {
  std::vector<T> result;

  for (int index : indices) {
    if (index >= 0 && index < vec.size()) {
      result.push_back(vec[index]);
    }
  }

  return result;
}

template<typename T>
std::vector<int> FindIndices(const std::vector<T>& vector1, const std::unordered_set<T>& filter_set) {
  std::vector<int> indices;

  for (int i = 0; i < vector1.size(); ++i) {
    if (filter_set.find(vector1[i]) != filter_set.end()) {
      indices.push_back(i);
    }
  }

  return indices;
}

ConstraintTable::ConstraintTable(const Table& initTable) : table(initTable) {}

void ConstraintTable::Solve(Constraint& constraint) {
}

std::vector<std::string> Select(const Header& header, const std::unordered_set<Constraint>& constraints) {
  if (constraints.empty()) {
    return {};
  }
  return {};
}

// TODO(phuccuongngo99): Better documentation here
// Add new reader to the table
void ConstraintTable::AddNewUnaryConstraint(const Header& new_header, const Col& new_values) {
  if (table.empty()) {
    table[new_header] = new_values;
    return;
  }

  int new_values_size = new_values.size();
  int table_size = table.begin()->second.size();

  // Call RepeatElements for each existing Headers
  for (auto& [header, values] : table) {
    values = RepeatElements(values, new_values_size);
  }

  // Call RepeatVector for the new header
  table[new_header] = RepeatVector(new_values, table_size);
}

// TODO(phuccuongngo99): Consider a different type here maybe
void ConstraintTable::AddNewBinaryConstraint(const Header& new_header1, const Col& new_values1,
                                             const Header& new_header2, const Col& new_values2) {
  assert(new_values1.size() == new_values2.size() && "New values of binary constraint do not have the same size!");

  if (table.empty()) {
    table[new_header1] = new_values1;
    table[new_header2] = new_values2;
    return;
  }

  int new_values_size = new_values1.size();
  int table_size = table.begin()->second.size();

  // Call RepeatElements for each existing Headers
  for (auto& [header, values] : table) {
    values = RepeatElements(values, new_values_size);
  }

  // Call RepeatVector for the new header
  table[new_header1] = RepeatVector(new_values1, table_size);
  table[new_header2] = RepeatVector(new_values2, table_size);
}

// TODO(phuccuongngo99): This should take in unordered set instead that maybe better
void ConstraintTable::AddExistingUnaryConstraint(const Header& existing_header, const Col& new_values) {
  assert(table.find(existing_header) != table.end() && "Existing header not found in the table!");

  std::unordered_set<Cell> filter_set(new_values.begin(), new_values.end());
  std::vector<int> indices = FindIndices<Cell>(table[existing_header], filter_set);

  // Call ExtractFromIndices for each existing Headers
  for (auto& [header, values] : table) {
    values = ExtractFromIndices(values, indices);
  }
}

// TODO(phuccuongngo99): Take an unordered set of pair here is better
void ConstraintTable::AddExistingBinaryConstraint(const Header& existing_header1, const Col& new_values1,
                                                  const Header& existing_header2, const Col& new_values2) {
  assert(table.find(existing_header1) != table.end() && "Existing header not found in the table!");
  assert(table.find(existing_header2) != table.end() && "Existing header not found in the table!");
  assert(new_values1.size() == new_values2.size() && "New values of binary constraint do not have the same size!");
  std::vector<int> indices;

  Col& existing_values1 = table[existing_header1];
  Col& existing_values2 = table[existing_header2];
  for (int i = 0; i < existing_values1.size(); ++i) {
    for (int j = 0; j < new_values1.size(); ++j) {
      if (existing_values1[i] == new_values1[j] && existing_values2[i] == new_values2[j]) {
        indices.push_back(i);
      }
    }
  }

  // Call ExtractFromIndices for each existing Headers
  for (auto& [header, values] : table) {
    values = ExtractFromIndices(values, indices);
  }
}

// TODO(phuccuongngo99): Please add assert that new header doesn't exist in table too
void ConstraintTable::AddHalfExistingBinaryConstraint(const Header& existing_header, const Col& new_existing_values,
                                                      const Header& new_header, const Col& new_values) {
  assert(table.find(existing_header) != table.end() && "Existing header not found in the table!");
  assert(
      new_existing_values.size() == new_values.size()
          && "New values of binary constraint do not have the same size!");

  Table result;
  // Prepare result table structure
  for (const auto& [header, _] : table) {
    result[header] = {};
  }
  result[new_header] = {};

  // Commonly used variables
  Col& existing_col = table[existing_header];

  // Main logic
  for (int i = 0; i < existing_col.size(); ++i) {
    for (int j = 0; j < new_existing_values.size(); ++j) {
      if (existing_col[i] == new_existing_values[j]) {
        for (const auto& [current_header, values] : table) {
          result[current_header].push_back(values[i]);
        }
        result[new_header].push_back(new_values[j]);
      }
    }
  }

  table = result;
}
