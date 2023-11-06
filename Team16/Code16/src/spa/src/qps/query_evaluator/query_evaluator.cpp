#include "qps/query_evaluator/query_evaluator.h"

QueryEvaluator::QueryEvaluator(ReadFacade& pkb) : pkb(pkb) {}

std::unordered_set<std::string> BlankResult(bool is_boolean_select) {
  return (is_boolean_select) ?
         std::unordered_set<std::string>{"FALSE"}
                             : std::unordered_set<std::string>{};
}

std::unordered_set<std::string> QueryEvaluator::Evaluate(ParsedQuery& query) {
  bool is_boolean_select = query.selects.empty();

  ClauseGrouper clause_grouper;

  for (auto& clause_ptr : query.clauses) {
    bool is_boolean_clause = clause_ptr->GetSynonyms().empty();
    if (is_boolean_clause) {
      // Evaluate boolean clauses
      bool result = std::get<bool>(clause_ptr->Evaluate(pkb));
      // Flip the sign if it's a not clause
      if (clause_ptr->IsNot()) {
        result = !result;
      }
      if (!result) {
        return BlankResult(is_boolean_select);
      }
    } else {
      // Add non-boolean clauses to constraint table
      clause_grouper.addClause(std::move(const_cast<std::unique_ptr<Clause>&>(clause_ptr)));
    }
  }

  std::vector<ClauseGroup> clause_groups = clause_grouper.GetClauseGroupOrder();
  ConstraintTable aggregate_table;

  for (const auto& clause_group : clause_groups) {
    ConstraintTable table = clause_group.Evaluate(pkb);
    if (!table.IsValid()) {
      return BlankResult(is_boolean_select);
    } else {
      if (is_boolean_select) {
        continue;
      } else {
        std::unordered_set<ColName> selects =
            algorithm_utils::intersect(table.AvailableColName(), {query.selects.begin(), query.selects.end()});
        if (!selects.empty()) {
          table.Filter(query.selects);
          aggregate_table.JoinTable(table);
        }
      }
    }
  }

  // If we reach here, meaning we will get non-blank result
  return (is_boolean_select) ? std::unordered_set<std::string>{"TRUE"}
                             : aggregate_table.Select(query.selects);
}
