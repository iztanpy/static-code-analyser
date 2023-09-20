#include "qps/query_evaluator/query_evaluator.h"

QueryEvaluator::QueryEvaluator(ReadFacade& pkb) : pkb(pkb) {}

std::unordered_set<std::string> QueryEvaluator::Evaluate(const ParsedQuery& query) {
  // Get the list of result from this select first
  SelectClause select_clause = query.select;
  std::unordered_set<std::string> select_results = SelectEvaluator(pkb, select_clause).Evaluate().values;

  // If there's only select clause -> return select result
  if (query.such_that_clauses.empty() && query.pattern_clauses.empty()) {
    return select_results;
  }

  ConstraintTable constraint_table;

  // Evaluate such-that clauses, if at any point that the table is empty
  // then return empty result
  for (const auto& clausePtr : query.such_that_clauses) {
    try {
      Constraint constraint = clausePtr->Evaluate(pkb);
      constraint_table.Solve(constraint);
      if (constraint_table.IsEmpty()) {
        return {};
      }
    } catch (const QpsSemanticError& e) {
      throw e;
    }
  }

  // Evaluate pattern clauses, if at any point that the table is empty
  // then return empty result
  for (const auto& clausePtr : query.pattern_clauses) {
    try {
      Constraint constraint = clausePtr->Evaluate(pkb);
      constraint_table.Solve(constraint);
      if (constraint_table.IsEmpty()) {
        return {};
      }
    } catch (const QpsSemanticError& e) {
      throw e;
    }
  }

  // If it reaches here, that means there's something inside
  std::unordered_set<ColName> table_colnames = constraint_table.AvailableColName();

  if (table_colnames.find(select_clause.synonym) != table_colnames.end()) {
    return constraint_table.Select(select_clause.synonym);
  } else {
    return select_results;
  }
}
