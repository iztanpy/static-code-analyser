#include "qps/query_evaluator/query_evaluator.h"

QueryEvaluator::QueryEvaluator(ReadFacade& pkb) : pkb(pkb) {}

std::unordered_set<std::string> QueryEvaluator::Evaluate(const ParsedQuery& query) {
  // TODO: Tell sumian to pass back the updated data structure

  ConstraintTable constraint_table;

  // Evaluate clauses
  for (const auto& clausePtr : query.clauses) {
    Constraint constraint = clausePtr->Evaluate(pkb);
    constraint_table.Solve(constraint);
    if (!constraint_table.IsValid()) {
      return {};
    }
  }

  return constraint_table.Select(query.selects[0]);
}
