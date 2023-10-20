#include "qps/query_evaluator/constraint_solver/clause_group.h"

int ClauseGroup::Score() const {
  return score_;
}

ClauseGroup::ClauseGroup(ClauseSet& clauseSet) {
  size_t originalSize = clauseSet.size();

  // 1. Build a hash map between Synonym to Clauses that are connected to that Synonym.
  // Change the declaration of synonymToClauses to:
  std::unordered_map<Synonym, std::vector<Clause*>> synonymToClauses;

  // And when inserting:
  for (const auto& clause : clauseSet) {
    auto synonyms = clause->GetSynonyms();
    assert(synonyms.size() == 1 || synonyms.size() == 2);
    for (const auto& synonym : synonyms) {
      synonymToClauses[synonym].push_back(clause.get());
    }
  }

  // 2. Initiate the Priority Queue of Clauses called candidates.
  auto compare = [](const Clause* lhs, const Clause* rhs) {
    return lhs->Score() > rhs->Score();  // Min heap
  };
  std::priority_queue<Clause*, std::vector<Clause*>, decltype(compare)> candidates(compare);

  // 3. Find the Clause with the smallest score.
  auto it = std::min_element(clauseSet.begin(), clauseSet.end(),
                             [](const std::unique_ptr<Clause>& lhs, const std::unique_ptr<Clause>& rhs) {
                               return lhs->Score() < rhs->Score();
                             });

  candidates.push(it->get());

  std::unordered_set<Clause*> visited;

  while (!candidates.empty()) {
    auto current = candidates.top();
    candidates.pop();
    if (visited.find(current) != visited.end()) {
      continue;
    }

    visited.insert(current);

    // Find the unique_ptr in the set that matches the raw pointer
    auto it = std::find_if(clauseSet.begin(), clauseSet.end(),
                           [current](const std::unique_ptr<Clause>& ptr) {
                             return ptr.get() == current;
                           });

    if (it != clauseSet.end()) {
      clauses_.push_back(std::move(const_cast<std::unique_ptr<Clause>&>(*it)));
      clauseSet.erase(it);
    }

    // 4. Look up the HashMap, add all connected Clause to candidates PQ.
    for (const auto& synonym : current->GetSynonyms()) {
      for (const auto& connected : synonymToClauses[synonym]) {
        if (visited.find(connected) == visited.end()) {
          candidates.push(connected);
        }
      }
    }
  }

  // 5. Double check that length of result vector is the same length as the input ClauseSet
  assert(clauses_.size() == originalSize);
}
