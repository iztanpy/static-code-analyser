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
  // 0. Initialize data structures
  std::list<size_t> notBinaryClauses;
  std::unordered_set<Synonym> notBinarySynonyms;
  std::unordered_map<Synonym, std::list<size_t>> synonymToClauses;
  std::vector<size_t> finalOrder;

  size_t smallestNormalClause;
  int smallest_score = INT_MAX;

  std::priority_queue<size_t, std::vector<size_t>, std::function<bool(size_t, size_t)>> candidates(
      [&unordered_clauses](size_t a, size_t b) {
        return unordered_clauses[a]->Score() > unordered_clauses[b]->Score();
      });

  // 1. Separate not_binary_clauses from normal clauses & build unordered_map for normal clauses
  // Also find the smallest normal clause to add to priority queue
  for (size_t i = 0; i < unordered_clauses.size(); ++i) {
    auto& clause = unordered_clauses[i];
    auto synonyms = clause->GetSynonyms();
    if (clause->IsNot() && synonyms.size() == 2) {
      // Not binary clause
      notBinaryClauses.push_back(i);
      notBinarySynonyms.insert(synonyms.begin(), synonyms.end());
    } else {
      // Logic to find the smallest normal clause
      int curr_score = clause->Score();
      if (curr_score < smallest_score) {
        smallest_score = curr_score;
        smallestNormalClause = i;
      }
      // Logic to build unordered_map
      for (const auto& synonym : synonyms) {
        synonymToClauses[synonym].push_back(i);
      }
    }
  }

  // 2.
  candidates.push(smallestNormalClause);
  std::unordered_set<size_t> insertedClauses = {smallestNormalClause};
  std::unordered_set<Synonym> visitedSynonyms;

  while (!candidates.empty()) {
    // Pop and add to finalOrder
    size_t curr = candidates.top();
    candidates.pop();

    // Add to finalOrder, update visitedSynonyms
    auto curr_synonyms = unordered_clauses[curr]->GetSynonyms();
    std::unordered_set<Synonym> new_visited_synomyms = {};
    for (const auto& synonym : curr_synonyms) {
      if (visitedSynonyms.find(synonym) == visitedSynonyms.end()) {
        new_visited_synomyms.insert(synonym);
      }
    }
    finalOrder.push_back(curr);
    visitedSynonyms.insert(curr_synonyms.begin(), curr_synonyms.end());

    // Add to candidates PQ
    for (const auto& synonym : new_visited_synomyms) {
      auto& clauses = synonymToClauses[synonym];
      for (auto& clause : clauses) {
        if (insertedClauses.find(clause) == insertedClauses.end()) {
          candidates.push(clause);
          insertedClauses.insert(clause);
        }
      }
      clauses = {};  // Clear the list
    }

    // Add all eligible not binary clauses to finalOrder
    if (!new_visited_synomyms.empty()) {
      notBinaryClauses.remove_if(
          [&visitedSynonyms, &finalOrder, &unordered_clauses]
              (size_t clause) {
            auto synonyms = unordered_clauses[clause]->GetSynonyms();
            if (algorithm_utils::is_subset(synonyms, visitedSynonyms)) {
              finalOrder.push_back(clause);
              visitedSynonyms.insert(synonyms.begin(), synonyms.end());
              return true;
            }
            return false;
          });
    }

    // Disjoint set connected only by not_binary_clause
    // This is because candidates has no more clauses to add but not all clauses
    // has been added to finalOrder
    if (candidates.empty() && finalOrder.size() != unordered_clauses.size()) {
      // Loop through unordered_map
      for (const auto& pair : synonymToClauses) {
        if (notBinarySynonyms.find(pair.first) != notBinarySynonyms.end()
            && !pair.second.empty()) {
          // If synonym is not visited, add all clauses to finalOrder
          for (const auto& clause : pair.second) {
            candidates.push(clause);
            insertedClauses.insert(clause);
          }
          break;  // Only need to do this once
        }
      }
    }
  }

  // Reorder unordered_clauses to private field clauses_
  assert(finalOrder.size() == unordered_clauses.size());
  for (auto& clause_idx : finalOrder) {
    clauses_.push_back(std::move(unordered_clauses[clause_idx]));
  }
}
