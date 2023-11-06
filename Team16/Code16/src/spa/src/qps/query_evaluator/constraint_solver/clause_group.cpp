#include "qps/query_evaluator/constraint_solver/clause_group.h"

// For just the average score of clauses inside
int ClauseGroup::Score() const {
  int score = 0;
  for (const auto& clause : clauses_) {
    score += clause->Score();
  }
  if (clauses_.empty()) return 0;
  return score / clauses_.size();
}

ConstraintTable ClauseGroup::Evaluate(ReadFacade& pkb_reader) const {
  ConstraintTable table;
  for (const auto& clause : clauses_) {
    Constraint constraint = clause->Evaluate(pkb_reader);
    table.Solve(constraint, clause->IsNot());
    if (!table.IsValid()) {
      return table;
    }
  }
  return table;
}

int ClauseGroup::SizeForTesting() const {
  return clauses_.size();
}

ClauseGroup::ClauseGroup(std::vector<std::unique_ptr<Clause>> unordered_clauses) {
  // First of all, just pick out all the binary, put them all at the back
  std::partition(unordered_clauses.begin(), unordered_clauses.end(),
                 [](const std::unique_ptr<Clause>& ptr) {
                   return !(ptr->IsNot());
                 });
  clauses_ = std::move(unordered_clauses);
}
