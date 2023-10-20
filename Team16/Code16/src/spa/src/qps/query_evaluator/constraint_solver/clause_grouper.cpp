#include "qps/query_evaluator/constraint_solver/clause_grouper.h"

// Adds a clause to the internal clause set.
void ClauseGrouper::addClause(std::unique_ptr<Clause> clause) {
  clauses_.insert(std::move(clause));
}

// Sorts and returns ClauseGroups based on their score.
std::vector<ClauseGroup> ClauseGrouper::GetClauseGroupOrder() {
  for (const auto& clause : clauses_) {
    Union(clause);
  }

  std::unordered_map<Synonym, ClauseSet> synonymToClauses;

  while (!clauses_.empty()) {
    auto it = clauses_.begin();
    Synonym root = Find(*(*it)->GetSynonyms().begin());
    synonymToClauses[root].insert(std::move(const_cast<std::unique_ptr<Clause>&>(*it)));
    clauses_.erase(it);
  }

  // Convert ClauseSet to ClauseGroup
  std::vector<ClauseGroup> clauseGroups;

  for (auto& pair : synonymToClauses) {
    ClauseGroup group(pair.second);
    clauseGroups.push_back(std::move(group));
  }

  // Sort Clause Group
  std::sort(clauseGroups.begin(), clauseGroups.end(),
            [](const ClauseGroup& a, const ClauseGroup& b) {
              return a.Score() < b.Score();
            });

  return clauseGroups;
}

Synonym ClauseGrouper::Find(const Synonym& syn) {
  if (map_.find(syn) == map_.end() || map_[syn] == syn) {
    return map_[syn] = syn;
  }

  return map_[syn] = Find(map_[syn]);
}

void ClauseGrouper::Union(const std::unique_ptr<Clause>& clause) {
  auto synonyms = clause->GetSynonyms();

  if (synonyms.size() == 1) {
    Find(*synonyms.begin());
  } else {
    Synonym root1 = Find(*synonyms.begin());
    Synonym root2 = Find(*(++synonyms.begin()));

    if (root1 != root2) {
      map_[root1] = root2;
    }
  }
}
