#include "qps/query_evaluator/result_formatter.h"

template<typename KeyType, typename ValueType>
std::unordered_set<KeyType>
GetKeys(const std::unordered_map<KeyType, ValueType>& umap) {
  std::unordered_set<KeyType> keySet;
  for (const auto& kv : umap) {
    keySet.insert(kv.first);
  }
  return keySet;
}

// Recursive to print all combinations with a row of our Constraint Table
void CombinationPerRow(
    const std::unordered_map<ColName, std::vector<Cell>>& umap,
    const std::unordered_map<ColName, Cell>& row,
    const std::vector<ColName>& order, int order_index,
    std::vector<Cell>& current_combination,
    std::unordered_set<std::string>& combinations) {

  // Base case: If the order_index reaches the end of the order vector, make the result
  // and add the combination to our result set
  if (order_index == order.size()) {
    std::string combination;
    for (const auto& str : current_combination) {
      combination += str + " ";
    }
    combinations.insert(combination);
    return;
  }

  const auto& key = order[order_index];

  if (umap.find(key) == umap.end()) {
    // If the key is not found, that means it's in the row
    current_combination[order_index] = row.at(key);
    CombinationPerRow(umap, row, order, order_index + 1, current_combination, combinations);
    return;
  }

  // Loop through all elements of the current key's vector
  for (const auto& str : umap.at(key)) {
    current_combination[order_index] = str;
    CombinationPerRow(umap, row, order, order_index + 1, current_combination, combinations);
    current_combination[order_index] = "";
  }
}

namespace ResultFormatter {
std::unordered_set<std::string> CombinationWithTable(
    const std::unordered_map<ColName, std::vector<Cell>>& non_existing_values,
    const Table& table, const std::vector<ColName>& order) {
  // Add assert statement here

  // 1. Extract the headers/keys from non_existing_values and table
  std::unordered_set<ColName> non_existing_colnames = GetKeys(non_existing_values);
  std::unordered_set<ColName> existing_colnames = GetKeys(table);

  // 2. Assert that there's no overlap between non_existing_values and table
  for (const auto& key : non_existing_colnames) {
    assert(existing_colnames.find(key) == existing_colnames.end() &&
        "Overlap detected between non_existing_values and table headers!");
  }

  // 3. Assert the union of non_existing_values and table headers are within order
  std::unordered_set<ColName> order_keys(order.begin(), order.end());
  for (const auto& key : existing_colnames) {
    non_existing_colnames.insert(key);
  }

  assert(non_existing_colnames == order_keys &&
      "The union of non_existing_values and table headers doesn't match the order!");

  std::unordered_set<std::string> combinations;

  size_t num_row = table.begin()->second.size();
  for (size_t row_i = 0; row_i < num_row; ++row_i) {
    std::unordered_map<ColName, Cell> row;

    for (auto& [col_name, values] : table) {
      row[col_name] = values[row_i];
    }

    size_t vec_size = non_existing_values.size() + table.size();
    std::vector<std::string> current_combination(vec_size, "");
    CombinationPerRow(non_existing_values, row, order, 0, current_combination, combinations);
  }

  return combinations;
}
}  // namespace ResultFormatter
