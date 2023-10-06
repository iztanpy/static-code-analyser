#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>

class CallStore {
 private:
    typedef std::string procedure;
    std::unordered_map<procedure, std::unordered_set<procedure>> callTable;
    std::unordered_map<procedure, std::unordered_set<procedure>> callTableReverse;
    std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar;
    std::unordered_map<procedure, std::unordered_set<procedure>> callTableStarReverse;

 public:
    CallStore();

    void CallStore::storeCalls(std::unordered_map<procedure, std::unordered_set<procedure>> callTable);

    std::unordered_set<procedure> CallStore::getCallChildren(procedure p);

    std::unordered_set<procedure> CallStore::getCallParents(procedure p);

    std::unordered_set<procedure> CallStore::getCallStarChildren(procedure p);

    std::unordered_set<procedure> CallStore::getCallStarParents(procedure p);
};
