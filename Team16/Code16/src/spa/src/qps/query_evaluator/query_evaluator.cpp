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

  ConstraintSolver constraint_solver;

  // Simple hacky implementation for now, the final flow should be as above
  SelectClause select_clause = query.select;
  SelectEvaluator select_evaluator(pkb, select_clause);

  // When there's only select clause
  if (query.such_that_clauses.empty()) {
    UnaryConstraint constraint = select_evaluator.Evaluate();

    return {constraint.values.begin(), constraint.values.end()};
  }

//  for (const SuchThatClause& suchthat_clause : query.suchthat_clauses) {
//    SuchThatEvaluator suchthat_evaluator(pkb, suchthat_clause);
//    Constraint constraint = suchthat_evaluator.evaluate();
//    constraint_solver.Solve(constraint);
//  }

  return {};
}
