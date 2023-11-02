#include "qps/query_evaluator/constraint_solver/clause_grouper.h"

// Adds a clause to the internal clause set.
void ClauseGrouper::addClause(std::unique_ptr<Clause> clause) {
  std::unordered_set clause_synonyms = clause->GetSynonyms();
  if (clause->IsNot()) {
    not_synonyms_.insert(clause_synonyms.begin(), clause_synonyms.end());
  } else {
    synonyms_.insert(clause_synonyms.begin(), clause_synonyms.end());
  }

  clauses_.insert(std::move(clause));
}

void ClauseGrouper::TopUpSelectClause() {
  for (const auto& not_synonym : not_synonyms_) {
    if (synonyms_.find(not_synonym) == synonyms_.end()) {
      std::unique_ptr<Clause> select_clause =
          std::make_unique<SelectClause>(
              AttrRef(
                  Declaration{not_synonym, DesignEntity::VARIABLE},
                  AttrName::NONE));
      clauses_.insert(std::move(select_clause));
    }
  }
}

// Sorts and returns ClauseGroups based on their score.
std::vector<ClauseGroup> ClauseGrouper::GetClauseGroupOrder() {
  // Top up Select Clause
  TopUpSelectClause();

  for (const auto& clause : clauses_) {
    Union(clause);
  }

  std::unordered_map<Synonym, ClauseSet> synonymToClauses;

  for (auto& clause_ptr : clauses_) {
    Synonym root = Find(*clause_ptr->GetSynonyms().begin());
    synonymToClauses[root].insert(std::move(const_cast<std::unique_ptr<Clause>&>(clause_ptr)));
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
