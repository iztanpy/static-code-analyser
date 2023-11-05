#include "qps/query_evaluator/constraint_solver/clause_grouper.h"

// Adds a clause to the internal clause set.
void ClauseGrouper::addClause(std::unique_ptr<Clause> clause) {
  clauses_.push_back(std::move(clause));
}

// Sorts and returns ClauseGroups based on their score.
std::vector<ClauseGroup> ClauseGrouper::GetClauseGroupOrder() {
  // Top up Select Clause
  TopUpSelectClause();

  // Union
  for (const auto& clause : clauses_) {
    Union(clause);
  }

  // Find
  std::unordered_map<Synonym, std::vector<std::unique_ptr<Clause>>> synonymToClauses;

  for (auto& clause_ptr : clauses_) {
    Synonym root = Find(*clause_ptr->GetSynonyms().begin());
    synonymToClauses[root].push_back(std::move(clause_ptr));
  }

  // Convert ClauseSet to ClauseGroup
  std::vector<ClauseGroup> clauseGroups;

  for (auto& pair : synonymToClauses) {
    ClauseGroup group(std::move(pair.second));
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

void ClauseGrouper::TopUpSelectClause() {
  std::unordered_set<Declaration> normal_decls;
  std::unordered_set<Declaration> not_decls;

  for (const auto& clause : clauses_) {
    auto decls = clause->ComputeSynonyms();
    if (clause->IsNot()) {
      not_decls.insert(decls.begin(), decls.end());
    } else {
      normal_decls.insert(decls.begin(), decls.end());
    }
  }

  for (const auto& not_decl : not_decls) {
    if (normal_decls.find(not_decl) == normal_decls.end()) {
      clauses_.push_back(
          std::make_unique<SelectClause>(
              AttrRef(not_decl, AttrName::NONE)));
    }
  }
}
