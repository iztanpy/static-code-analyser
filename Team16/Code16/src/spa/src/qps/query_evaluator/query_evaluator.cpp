#include "qps/query_evaluator/query_evaluator.h"

QueryEvaluator::QueryEvaluator(ReadFacade& pkb) : pkb(pkb) {}

std::unordered_set<std::string> QueryEvaluator::Evaluate(const ParsedQuery& query) {
  // Get the list of result from this select first
  SelectClause select_clause = query.select;
  std::unordered_set<std::string> select_results = select_clause.Evaluate(pkb).values;

  ConstraintTable constraint_table;

  // Evaluate such-that clauses
  for (const auto& clausePtr : query.such_that_clauses) {
    Constraint constraint = clausePtr->Evaluate(pkb);
    constraint_table.Solve(constraint);
    if (!constraint_table.IsValid()) {
      return {};
    }
  }

  // Evaluate pattern clauses
  for (const auto& clausePtr : query.pattern_clauses) {
    Constraint constraint = clausePtr->Evaluate(pkb);
    constraint_table.Solve(constraint);
    if (!constraint_table.IsValid()) {
      return {};
    }
  }

  // If it reaches here, that means there's something inside
  std::unordered_set<ColName> table_colnames = constraint_table.AvailableColName();

  if (table_colnames.find(select_clause.declaration.synonym) != table_colnames.end()) {
    return constraint_table.Select(select_clause.declaration.synonym);
  } else {
    return select_results;
  }
}
