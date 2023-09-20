#include "qps/query_evaluator/query_evaluator.h"

QueryEvaluator::QueryEvaluator(ReadFacade& pkb) : pkb(pkb) {}

std::vector<std::string> QueryEvaluator::Evaluate(const ParsedQuery& query) {
  // TODO(phuccuongngo99): Iterate through all SuchThatClauses in query
  // Look at what type of SuchThatClause it is
  // Construct the appropriate SuchThatEvaluator (Factory method)
  // Call evaluate() on the SuchThatEvaluator to get back a Constraint
  // Add the Constraint to the list of Constraints
  // Pass this list of Constraints to Constraint Solver
  // Then call Constraint_Solver.solve to get back list of result

  ConstraintTable constraint_table;

  // Get the list of result from this select first
  SelectClause select_clause = query.select;
  SelectEvaluator select_evaluator(pkb, select_clause);
  UnaryConstraint select_constraint = select_evaluator.Evaluate();
  std::vector<std::string> select_results = {select_constraint.values.begin(),
                                             select_constraint.values.end()};

  // When there's only select clause
  if (query.such_that_clauses.empty() && empty()) {
    UnaryConstraint constraint = select_evaluator.Evaluate();

    return {constraint.values.begin(), constraint.values.end()};
  }

  for (const auto& clausePtr : query.such_that_clauses) {
    try {
      Constraint constraint = clausePtr->Evaluate(pkb);
      constraint_table.Solve(constraint);
    } catch (const QpsSemanticError& e) {
      throw e;
    }
  }

  Table table = constraint_table.GetTableForTesting();
  if (table.empty() || table.begin()->first.empty()) {
    return {};
  }

  // If it reaches here, that means there's something inside
  std::unordered_set<ColName> col_names = constraint_table.AvailableColName();

  if (col_names.find(select_clause.synonym) != col_names.end()) {
    std::unordered_set<std::string> results = constraint_table.Select(select_clause.synonym);
    return {results.begin(), results.end()};
  } else {
    UnaryConstraint constraint = select_evaluator.Evaluate();
    return {constraint.values.begin(), constraint.values.end()};
  }
}
