#include "qps/query_evaluator/constraint_solver/constraint_table.h"

// {1, 2, 3}, 2 -> {1, 1, 2, 2, 3, 3}
std::vector<std::string> RepeatElements(const std::vector<std::string>& vec, size_t n) {
  std::vector<std::string> result;
  result.reserve(vec.size() * n);
  for (const std::string& val : vec) {
    for (int i = 0; i < n; ++i) {
      result.emplace_back(val);
    }
  }
  return result;
}

std::vector<std::string> RepeatVector(const std::vector<std::string>& vec, size_t n) {
  std::vector<std::string> result;
  result.reserve(vec.size() * n);
  for (int i = 0; i < n; ++i) {
    for (const std::string& val : vec) {
      result.emplace_back(val);
    }
  }
  return result;
}

template<typename T>
std::vector<T> SelectByIndex(const std::vector<T>& vec, const std::vector<std::size_t>& indices) {
  std::vector<T> result;
  result.reserve(indices.size());

  for (size_t index : indices) {
    result.emplace_back(vec[index]);
  }

  return result;
}

std::string RowToString(const Table& table, int row_index, const std::vector<ColName>& keys) {
  std::ostringstream oss;
  for (size_t i = 0; i < keys.size(); ++i) {
    const auto& key = keys[i];
    auto it = table.find(key);
    if (it != table.end() && row_index < it->second.size()) {
      if (i > 0) {
        oss << " ";
      }
      oss << it->second[row_index];
    }
  }
  return oss.str();
}

ConstraintTable::ConstraintTable() {
  table = {};
}

Table ConstraintTable::GetTableForTesting() {
  return table;
}

bool ConstraintTable::IsValid() {
  return !table.empty() && !table.begin()->second.empty();
}

void ConstraintTable::Solve(const UnaryConstraint& constraint, bool is_not) {
  ColName col_name = constraint.col_name;
  if (table.find(col_name) == table.end()) {
    // Add new ColName to the table
    return AddNewUnaryConstraint(constraint, is_not);
  }
  return AddExistingUnaryConstraint(constraint, is_not);
}

void ConstraintTable::Solve(const BinaryConstraint& constraint, bool is_not) {
  ColName col_name1 = constraint.pair_col_names.first;
  ColName col_name2 = constraint.pair_col_names.second;

  if (table.find(col_name1) == table.end() && table.find(col_name2) == table.end()) {
    // Add new ColName to the table
    return AddNewBinaryConstraint(constraint, is_not);
  }

  if (table.find(col_name1) != table.end() && table.find(col_name2) != table.end()) {
    // Add existing ColName pair to the table
    return AddExistingBinaryConstraint(constraint, is_not);
  }

  return AddHalfExistingBinaryConstraint(constraint, is_not);
}

void ConstraintTable::Solve(const bool constraint, bool is_not) {
  assert(false && "Solve(bool) should not be called");
}

void ConstraintTable::Solve(Constraint& constraint, bool is_not) {
  std::visit([this, is_not](const auto& obj) {
    Solve(obj, is_not);
  }, constraint);
}

std::unordered_set<std::string> ConstraintTable::Select(const std::vector<ColName>& selects) {
  // Ensure all keys in the vector are in the map
  for (const auto& key : selects) {
    assert(table.find(key) != table.end() && "Key not found in map");
  }

  std::unordered_set < std::string > result;

  if (selects.empty()) {
    return result;
  }

  size_t length = table.at(selects[0]).size();
  for (size_t i = 0; i < length; ++i) {
    result.emplace(RowToString(table, i, selects));
  }
  return result;
}

std::unordered_set<ColName> ConstraintTable::AvailableColName() {
  std::unordered_set < ColName > result;
  for (const auto& [col_name, _] : table) {
    result.emplace(col_name);
  }
  return result;
}

void ConstraintTable::AddNewUnaryConstraint(const UnaryConstraint& constraint, bool is_not) {
  if (is_not)
    throw std::runtime_error("New unary constraint should not be negated");

  assert(table.find(constraint.col_name) == table.end() && "Supposedly new ColName found in the table!");

  ColName new_header = constraint.col_name;
  std::vector<Cell> new_values = {constraint.values.begin(), constraint.values.end()};

  // If table is empty, just add the new header and values
  if (table.empty()) {
    table[new_header] = std::move(new_values);
    return;
  }

  int new_values_size = new_values.size();
  int table_size = table.begin()->second.size();

  // Call RepeatElements for each existing Headers
  for (auto& [header, values] : table) {
    values = std::move(RepeatElements(values, new_values_size));
  }

  // Call RepeatVector for the new header
  table[new_header] = std::move(RepeatVector(new_values, table_size));
}

void ConstraintTable::AddNewBinaryConstraint(const BinaryConstraint& constraint, bool is_not) {
  if (is_not)
    throw std::runtime_error("New binary constraint should not be negated");

  assert(table.find(constraint.pair_col_names.first) == table.end()
             || table.find(constraint.pair_col_names.second) == table.end()
                 && "Supposedly new pair of ColNames found in the table!");

  ColName col_name1 = constraint.pair_col_names.first;
  ColName col_name2 = constraint.pair_col_names.second;
  std::vector<Cell> new_values1;
  std::vector<Cell> new_values2;

  for (const auto& [value1, value2] : constraint.pair_values) {
    new_values1.emplace_back(value1);
    new_values2.emplace_back(value2);
  }

  if (table.empty()) {
    table[col_name1] = std::move(new_values1);
    table[col_name2] = std::move(new_values2);
    return;
  }

  int new_values_size = new_values1.size();
  int table_size = table.begin()->second.size();

  // Call RepeatElements for each existing Headers
  for (auto& [header, values] : table) {
    values = std::move(RepeatElements(values, new_values_size));
  }

  // Call RepeatVector for the new header
  table[col_name1] = std::move(RepeatVector(new_values1, table_size));
  table[col_name2] = std::move(RepeatVector(new_values2, table_size));
}

void ConstraintTable::AddExistingUnaryConstraint(const UnaryConstraint& existing_constraint, bool is_not) {
  assert(table.find(existing_constraint.col_name) != table.end() && "Existing ColName not found in the table!");

  ColName existing_header = existing_constraint.col_name;
  std::unordered_set < Cell > filter_values = existing_constraint.values;

  std::vector<std::size_t> filter_indices;
  std::vector<Cell>& existing_values = table[existing_header];

  for (std::size_t i = 0; i < existing_values.size(); ++i) {
    bool value_found = filter_values.find(existing_values[i]) != filter_values.end();
    if (is_not != value_found) {
      filter_indices.emplace_back(i);
    }
  }

  // Call SelectByIndex for each existing Headers
  for (auto& [header, values] : table) {
    values = std::move(SelectByIndex(values, filter_indices));
  }
}

void ConstraintTable::AddExistingBinaryConstraint(const BinaryConstraint& existing_constraint, bool is_not) {
  ColName existing_header1 = existing_constraint.pair_col_names.first;
  ColName existing_header2 = existing_constraint.pair_col_names.second;

  assert(table.find(existing_header1) != table.end() && "Existing ColName not found in the table!");
  assert(table.find(existing_header2) != table.end() && "Existing ColName not found in the table!");

  std::vector<std::size_t> indices;

  std::vector<Cell>& existing_values1 = table[existing_header1];
  std::vector<Cell>& existing_values2 = table[existing_header2];

  for (int i = 0; i < existing_values1.size(); ++i) {
    std::pair<Cell, Cell> existing_pair{existing_values1[i], existing_values2[i]};

    bool value_found = existing_constraint.pair_values.find(existing_pair) != existing_constraint.pair_values.end();

    if (is_not != value_found) {
      indices.emplace_back(i);
    }
  }

  // Call SelectByIndex for each existing Headers
  for (auto& [header, values] : table) {
    values = std::move(SelectByIndex(values, indices));
  }
}

void ConstraintTable::AddHalfExistingBinaryConstraint(const BinaryConstraint& constraint, bool is_not) {
  if (is_not)
    throw std::runtime_error("Half existing binary constraint should not be negated");

  ColName existing_colname;
  ColName new_colname;
  std::unordered_map < Cell, std::unordered_set < Cell >> new_values;

  if (table.find(constraint.pair_col_names.first) != table.end()
      && table.find(constraint.pair_col_names.second) == table.end()
      ) {
    existing_colname = constraint.pair_col_names.first;
    new_colname = constraint.pair_col_names.second;

    for (const auto& [value1, value2] : constraint.pair_values) {
      new_values[value1].emplace(value2);
    }

  } else if (table.find(constraint.pair_col_names.first) == table.end()
      && table.find(constraint.pair_col_names.second) != table.end()) {
    existing_colname = constraint.pair_col_names.second;
    new_colname = constraint.pair_col_names.first;

    for (const auto& [value1, value2] : constraint.pair_values) {
      new_values[value2].emplace(value1);
    }
  } else {
    assert(false && "Both ColNames are either both existent or non-existent in the table");
  }

  Table result;

  // Prepare result table structure
  for (const auto& [colname, _] : table) {
    result[colname] = {};
  }
  result[new_colname] = {};

  std::vector<Cell>& existing_col = table[existing_colname];

  // Main logic
  for (int i = 0; i < existing_col.size(); ++i) {
    Cell existing_value = existing_col[i];

    if (new_values.find(existing_value) != new_values.end()) {
      const auto& new_col_values = new_values[existing_value];

      for (const auto& new_col_value : new_col_values) {
        for (const auto& [current_colname, values] : table) {
          result[current_colname].emplace_back(values[i]);
        }
        result[new_colname].emplace_back(new_col_value);
      }
    }
  }

  table = std::move(result);
}

void ConstraintTable::JoinTable(const ConstraintTable& constraint_table) {
  // Assert that there's no overlap between the ColNames
  for (const auto& [key, _] : constraint_table.table) {
    assert(table.find(key) == table.end());
  }

  if (table.empty()) {
    table = constraint_table.table;
    return;
  }

  size_t table_len = table.begin()->second.size();
  size_t constraint_table_len = constraint_table.table.begin()->second.size();

  // Transform current table using function A
  for (auto& [key, value] : table) {
    value = RepeatVector(value, constraint_table_len);
  }

  // Transform values from constraint_table using function B and emplace directly into table
  for (const auto& [key, value] : constraint_table.table) {
    table[key] = RepeatElements(value, table_len);
  }
}

void ConstraintTable::Filter(const std::vector<ColName>& col_names) {
  // Create an unordered_set from the vector
  std::unordered_set < ColName > col_set(col_names.begin(), col_names.end());
  std::vector<ColName> dedup_colnames(col_set.begin(), col_set.end());

  std::unordered_set < std::string > seen_rows;

  for (auto it = table.begin(); it != table.end();) {
    if (col_set.find(it->first) == col_set.end()) {
      it = table.erase(it);  // erase returns the iterator to the next element
    } else {
      ++it;
    }
  }

  Table result;
  for (const auto& [colname, _] : table) {
    result[colname] = {};
  }

  size_t table_len = table.begin()->second.size();
  for (size_t i = 0; i < table_len; ++i) {
    std::string row = RowToString(table, i, dedup_colnames);
    if (seen_rows.find(row) == seen_rows.end()) {
      seen_rows.emplace(row);
      for (const auto& [colname, values] : table) {
        result[colname].emplace_back(values[i]);
      }
    }
  }

  table = std::move(result);
}
