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

void CombinationPerRow(
    const std::unordered_map<ColName, std::vector<Cell>>& umap,
    const std::unordered_map<ColName, Cell>& row,
    const std::vector<ColName>& order, int order_index,
    std::vector<Cell>& current_combination) {

  // Base case: If the order_index reaches the end of the order vector, print
  // the current combination
  if (order_index == order.size()) {
    for (const auto& str : current_combination) {
      std::cout << str << " ";
    }
    std::cout << std::endl;
    return;
  }

  const auto& key = order[order_index];
  if (umap.find(key) == umap.end()) {
    // That means it's in the group values
    current_combination[order_index] = row.at(key);
    // If the key is not found, move to the next one
    CombinationPerRow(umap, row, order, order_index + 1, current_combination);
    return;
  }

  // Loop through all elements of the current key's vector
  for (const auto& str : umap.at(key)) {
    current_combination[order_index] = str;
    CombinationPerRow(umap, row, order, order_index + 1, current_combination);
    current_combination[order_index] = "";
  }
}

namespace ResultFormatter {
// Recursive function to print all combinations

void CombinationWithTable(
    const std::unordered_map<ColName, std::vector<Cell>>& umap,
    const Table& table, const std::vector<ColName>& order) {
  // Add assert statement here

  // 1. Extract the headers/keys from umap and table
  std::unordered_set<ColName> umap_keys = GetKeys(umap);
  std::unordered_set<ColName> table_keys = GetKeys(table);

  // 2. Assert that there's no overlap between umap and table
  for (const auto& key : umap_keys) {
    assert(table_keys.find(key) == table_keys.end() &&
        "Overlap detected between umap and table headers!");
  }

  // 3. Assert the union of umap and table headers are within order
  std::unordered_set<ColName> order_keys(order.begin(), order.end());
  for (const auto& key : table_keys) {
    umap_keys.insert(key);
  }

  assert(umap_keys == order_keys &&
      "The union of umap and table headers doesn't match the order!");

  size_t num_row = table.begin()->second.size();
  for (size_t row_i = 0; row_i < num_row; ++row_i) {
    std::unordered_map<ColName, Cell> row;

    for (auto& [col_name, values] : table) {
      row[col_name] = values[row_i];
    }

    size_t vec_size = umap.size() + table.size();
    std::vector<std::string> current_combination(vec_size, "");
    CombinationPerRow(umap, row, order, 0, current_combination);
  }
}
}  // namespace ResultFormatter
