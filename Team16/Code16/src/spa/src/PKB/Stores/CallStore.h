#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <utility>
#include "utils/hash_utils.h"
#include "utils/entity_types.h"
#include "qps/declaration.h"

class CallStore {
 private:
  typedef std::string procedure;
  std::unordered_map<procedure, std::unordered_set<procedure>> callTable;
  std::unordered_map<procedure, std::unordered_set<procedure>> callTableReverse;
  std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar;
  std::unordered_map<procedure, std::unordered_set<procedure>> callTableStarReverse;

 public:
  CallStore();

  void storeCalls(std::unordered_map<procedure, std::unordered_set<procedure>> callTable);

  std::unordered_map<procedure, std::unordered_set<procedure>> getCallStar();

  std::unordered_set<std::pair<procedure, procedure>, PairHash> call(StmtEntity procedure1,
                                                                     StmtEntity procedure2);

  std::unordered_set<procedure> call(StmtEntity procedure, Wildcard wildcard);

  std::unordered_set<procedure> call(Wildcard wildcard, StmtEntity procedure);

  std::unordered_set<procedure> getCallChildren(procedure p);

  std::unordered_set<procedure> getCallParents(procedure p);

  bool isCall(Wildcard wildcard, Wildcard wildcard2);

  bool isCall(Wildcard wildcard, procedure p);

  bool isCall(procedure p, Wildcard wildcard);

  bool isCall(procedure p, procedure p2);

  std::unordered_set<std::pair<procedure, procedure>, PairHash> callStar(StmtEntity procedure1,
                                                                         StmtEntity procedure2);

  std::unordered_set<procedure> callStar(StmtEntity procedure, Wildcard wildcard);

  std::unordered_set<procedure> callStar(Wildcard wildcard, StmtEntity procedure);

  std::unordered_set<procedure> getCallStarChildren(procedure p);

  std::unordered_set<procedure> getCallStarParents(procedure p);

  bool isCallStar(Wildcard wildcard, Wildcard wildcard2);

  bool isCallStar(Wildcard wildcard, procedure p);

  bool isCallStar(procedure p, Wildcard wildcard);

  bool isCallStar(procedure p, procedure p2);
};
