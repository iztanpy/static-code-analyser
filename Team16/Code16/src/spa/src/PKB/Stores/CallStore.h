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

    void storeCalls(std::unordered_map<procedure, std::unordered_set<procedure>> callTable);

    std::unordered_set<procedure> getCallChildren(procedure p);

    std::unordered_set<procedure> getCallParents(procedure p);

    std::unordered_set<procedure> getCallStarChildren(procedure p);

    std::unordered_set<procedure> getCallStarParents(procedure p);
};
