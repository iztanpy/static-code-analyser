#include "qps/query_evaluator/select_evaluator.h"

SelectEvaluator::SelectEvaluator(ReadFacade& pkb_reader, const SelectClause select_clause)
    : pkb_reader(pkb_reader), select_clause(select_clause) {}

UnaryConstraint SelectEvaluator::Evaluate() {
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
      case DesignEntity::CONSTANT:return pkb_reader.getConstants();

      case DesignEntity::VARIABLE:return pkb_reader.getVariables();

      default:throw std::runtime_error("Haven't implemented yet");
    }
  }();

  return UnaryConstraint{
      select_clause.synonym,
      values
  };
}
