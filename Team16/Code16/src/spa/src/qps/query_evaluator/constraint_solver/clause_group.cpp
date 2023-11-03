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

ClauseGroup::ClauseGroup(ClauseSet& clauseSet) {
  size_t originalSize = clauseSet.size();
  // 0. Seperate out binary NOT clause
  std::list<std::unique_ptr<Clause>> not_binary_clauses;
  ClauseSet filtered_clause_set;

  for (const auto& clause : clauseSet) {
    if (clause->IsNot() && clause->GetSynonyms().size() == 2) {
      not_binary_clauses.push_back(std::move(const_cast<std::unique_ptr<Clause>&>(clause)));
    } else {
      filtered_clause_set.insert(std::move(const_cast<std::unique_ptr<Clause>&>(clause)));
    }
  }

  // 1. Build a hash map between Synonym to Clauses that are connected to that Synonym.
  // Change the declaration of synonymToClauses to:
  std::unordered_map<Synonym, std::vector<Clause*>> synonymToClauses;

  // And when inserting:
  for (const auto& clause : filtered_clause_set) {
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

  // 3. Find the Clause with the smallest score, must not be NOT clause
  const std::unique_ptr<Clause>* min_clause = nullptr;
  int min_score = std::numeric_limits<int>::max();
  for (auto& clause : filtered_clause_set) {
    if (!clause->IsNot() && clause->Score() < min_score) {
      min_clause = &clause;
      min_score = clause->Score();
    }
  }

  if (min_clause != nullptr) {
    candidates.push(min_clause->get());
  } else {
    throw std::runtime_error("No non-NOT clause found in this ClauseGroup");
  }

  // 4. While candidates is not empty, pop the top Clause, add it to result vector.
  std::unordered_set<Clause*> visited;
  std::unordered_set<Synonym> visitedSynonyms;

  while (!candidates.empty()) {
    auto current = candidates.top();
    candidates.pop();
    if (visited.find(current) != visited.end()) {
      continue;
    }

    visited.insert(current);

    // Find the unique_ptr in the set that matches the raw pointer
    auto it = std::find_if(filtered_clause_set.begin(), filtered_clause_set.end(),
                           [current](const std::unique_ptr<Clause>& ptr) {
                             return ptr.get() == current;
                           });

    if (it != filtered_clause_set.end()) {
      clauses_.push_back(std::move(const_cast<std::unique_ptr<Clause>&>(*it)));
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
