#include "PKB/Stores/relation_store.h"
#include <unordered_map>
#include <utility>

typedef std::string variable;
typedef std::string constant;
typedef int statementNumber;
typedef std::string procedure;

relation_store::relation_store() {
  std::unordered_map<statementNumber, std::unordered_set<variable>> ForwardVariableStore;
  std::unordered_map<variable, std::unordered_set<statementNumber>> ReverseVariableStore;
  std::unordered_map<procedure, std::unordered_set<variable>> ForwardProcedureStore;
  std::unordered_map<variable, std::unordered_set<procedure>> ReverseProcedureStore;
}

void relation_store::storeRelation(std::unordered_map<statementNumber, std::unordered_set<variable>> relations) {
  this->ForwardVariableStore = relations;
  for (auto const& x : relations) {
    for (auto const& y : x.second) {
      this->ReverseVariableStore[y].insert(x.first);
    }
  }
}

void relation_store::storeRelationProcedures(std::unordered_map<procedure,
                                                                std::pair<int, int>> procedures,
                                             std::unordered_map<procedure,
                                                                std::unordered_set<procedure>> callTableStar) {
  for (auto const& x : procedures) {
    std::unordered_set<variable> variables;
    for (int i = x.second.first; i <= x.second.second; i++) {
      if (ForwardVariableStore.find(i) != ForwardVariableStore.end()) {
        for (auto const& y : ForwardVariableStore[i]) {
          variables.insert(y);
        }
      }
    }
    if (variables.size() > 0) {
      ForwardProcedureStore[x.first] = variables;
      for (auto const& y : variables) {
        ReverseProcedureStore[y].insert(x.first);
      }
    }
  }
  for (auto const& x : callTableStar) {
    for (auto const& y : x.second) {
      if (ForwardProcedureStore.find(y) != ForwardProcedureStore.end()) {
        for (auto const& z : ForwardProcedureStore[y]) {
          ForwardProcedureStore[x.first].insert(z);
          ReverseProcedureStore[z].insert(x.first);
        }
      }
    }
  }
}

void relation_store::storeRelationCalls(std::unordered_map<statementNumber, procedure> calls,
                                        std::unordered_map<statementNumber,
                                                           std::unordered_set<statementNumber>> callsParentMap) {
  for (auto const& x : calls) {
    if (ForwardProcedureStore[x.second].size() > 0) {
      ForwardVariableStore[x.first].insert(ForwardProcedureStore[x.second].begin(),
                                           ForwardProcedureStore[x.second].end());
      for (auto const& y : ForwardProcedureStore[x.second]) {
        ReverseVariableStore[y].insert(x.first);
      }

      for (auto parent : callsParentMap[x.first]) {
        if (ForwardVariableStore[parent].size() > 0) {
          ForwardVariableStore[parent].insert(ForwardProcedureStore[x.second].begin(),
                                              ForwardProcedureStore[x.second].end());
          for (auto const& y : ForwardProcedureStore[x.second]) {
            ReverseVariableStore[y].insert(parent);
          }
        }
      }
    }
  }

  for (auto const& x : calls) {
    for (auto parent : callsParentMap[x.first]) {
      if (ForwardVariableStore[x.first].size() > 0) {
        ForwardVariableStore[parent].insert(ForwardProcedureStore[x.second].begin(),
                                            ForwardProcedureStore[x.second].end());
        for (auto const& y : ForwardProcedureStore[x.second]) {
          ReverseVariableStore[y].insert(parent);
        }
      }
    }
  }
}

bool relation_store::isRelation(statementNumber statement, variable variable) {
  if (ForwardVariableStore.find(statement) != ForwardVariableStore.end()) {
    if (ForwardVariableStore[statement].find(variable) != ForwardVariableStore[statement].end()) {
      return true;
    }
  }
  return false;
}

bool relation_store::isRelation(statementNumber statement) {
  if (ForwardVariableStore.find(statement) != ForwardVariableStore.end()) {
    if (ForwardVariableStore[statement].size() > 0) {
      return true;
    }
  }
  return false;
}

std::unordered_set<statementNumber> relation_store::relates(relation_store::variable variable) {
  std::unordered_set<statementNumber> statements = ReverseVariableStore[variable];
  return statements;
}

std::unordered_set<variable> relation_store::relates(relation_store::statementNumber statement) {
  return ForwardVariableStore[statement];
}

bool relation_store::isRelation(procedure procedure) {
  if (ForwardProcedureStore.find(procedure) != ForwardProcedureStore.end()) {
    if (ForwardProcedureStore[procedure].size() > 0) {
      return true;
    }
  }
  return false;
}

std::unordered_set<variable> relation_store::relatesProcedureProc(procedure procedure) {
  return ForwardProcedureStore[procedure];
}

bool relation_store::isRelation(procedure procedure, variable variable) {
  if (ForwardProcedureStore.find(procedure) != ForwardProcedureStore.end()) {
    if (ForwardProcedureStore[procedure].find(variable) != ForwardProcedureStore[procedure].end()) {
      return true;
    }
  }
  return false;
}

std::unordered_set<procedure> relation_store::relatesProcedure() {
  std::unordered_set<procedure> procedures;
  for (auto const& x : ForwardProcedureStore) {
    if (x.second.size() > 0) {
      procedures.insert(x.first);
    }
  }
  return procedures;
}

std::unordered_set<procedure> relation_store::relatesProcedure(variable variable) {
  return ReverseProcedureStore[variable];
}

std::unordered_set<std::pair<procedure, variable>, PairHash> relation_store::relatesProcedurePair() {
  std::unordered_set<std::pair<procedure, variable>, PairHash> pairs;
  for (auto const& x : ForwardProcedureStore) {
    for (auto const& y : x.second) {
      pairs.insert(std::make_pair(x.first, y));
    }
  }
  return pairs;
}
