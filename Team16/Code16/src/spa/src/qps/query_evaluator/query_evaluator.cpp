#include "qps/query_evaluator/query_evaluator.h"

QueryEvaluator::QueryEvaluator(ReadFacade& pkb) : pkb(pkb) {}

std::unordered_set<std::string> BlankResult(bool is_boolean_select) {
  return (is_boolean_select) ?
         std::unordered_set < std::string > {"False"}
                             : std::unordered_set < std::string > {};
}

std::unordered_set<std::string> QueryEvaluator::Evaluate(ParsedQuery& query) {
  bool is_boolean_select = query.selects.empty();

  ClauseGrouper clause_grouper;

  while (!query.clauses.empty()) {
    auto clause = query.clauses.begin();

    bool is_boolean_clause = (*clause)->GetSynonyms().empty();
    if (is_boolean_clause) {
      // Evaluate boolean clauses
      bool result = std::get<bool>((*clause)->Evaluate(pkb));
      if (!result) {
        return BlankResult(is_boolean_select);
      }
    } else {
      // Add non-boolean clauses to constraint table
      clause_grouper.addClause(std::move(const_cast<std::unique_ptr<Clause>&>(*clause)));
    }
    query.clauses.erase(clause);
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
        aggregate_table.JoinTable(table);
      }
    }
  }

  // If we reach here, meaning we will get non-blank result
  return (is_boolean_select) ? std::unordered_set < std::string > {"True"}
                             : aggregate_table.Select(query.selects);
}
