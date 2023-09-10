#include "qps/query_evaluator/select_evaluator.h"

SelectEvaluator::SelectEvaluator(ReadFacade& pkb_reader, SelectClause& select_clause)
    : pkb_reader(pkb_reader), select_clause(select_clause) {}

Constraint SelectEvaluator::evaluate() {
  auto values = [&]() -> std::unordered_set<std::string> {
    switch (select_clause.design_entity) {
      case DesignEntity::ASSIGN: {
        std::unordered_set<int> assigns = pkb_reader.getAllAssigns();

        // Convert from int to string
        std::unordered_set<std::string> assigns_string;

        for (const int& line_number : assigns) {
          assigns_string.insert(std::to_string(line_number));
        }
        return assigns_string;
      }
      case DesignEntity::CONSTANT:return pkb_reader.getAllConstants();

      case DesignEntity::VARIABLE:return pkb_reader.getAllVariables();

      default:throw std::runtime_error("Haven't implemented yet");
    }
  }();

  std::unordered_set<std::pair<std::string, std::string>, PairHash> results;
  for (const std::string& value : values) {
    results.insert(std::make_pair(value, value));
  }
  return {
      std::make_pair(select_clause.synonym, select_clause.synonym),
      results
  };
}
