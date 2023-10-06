#include "CallStore.h"
#include <stack>

typedef std::string procedure;

CallStore::CallStore() {
    this->callTable = std::unordered_map<procedure, std::unordered_set<procedure>>();
    this->callTableReverse = std::unordered_map<procedure, std::unordered_set<procedure>>();
    this->callTableStar = std::unordered_map<procedure, std::unordered_set<procedure>>();
    this->callTableStarReverse = std::unordered_map<procedure, std::unordered_set<procedure>>();
}

void CallStore::storeCalls(std::unordered_map<procedure, std::unordered_set<procedure>> callTable) {
    this->callTable = callTable;

    for (auto it = callTable.begin(); it != callTable.end(); ++it) {
        procedure caller = it->first;
        std::unordered_set<procedure> callees = it->second;

        for (auto it2 = callees.begin(); it2 != callees.end(); ++it2) {
            procedure callee = *it2;
            this->callTableReverse[callee].insert(caller);
        }
    }

    for (auto it = callTable.begin(); it != callTable.end(); ++it) {
        procedure caller = it->first;
        std::unordered_set<procedure> callees = it->second;

        for (auto it2 = callees.begin(); it2 != callees.end(); ++it2) {
            procedure callee = *it2;
            this->callTableStar[caller].insert(callee);
            this->callTableStarReverse[callee].insert(caller);
        }
    }

    std::stack<procedure> callStack;
    for (auto it = callTable.begin(); it != callTable.end(); ++it) {
        procedure caller = it->first;
        std::unordered_set<procedure> callees = it->second;

        for (auto it2 = callees.begin(); it2 != callees.end(); ++it2) {
            procedure callee = *it2;
            callStack.push(callee);
        }

        while (!callStack.empty()) {
            procedure callee = callStack.top();
            callStack.pop();

            std::unordered_set<procedure> calleeCallees = callTable[callee];
            for (auto it3 = calleeCallees.begin(); it3 != calleeCallees.end(); ++it3) {
                procedure calleeCallee = *it3;
                this->callTableStar[caller].insert(calleeCallee);
                this->callTableStarReverse[calleeCallee].insert(caller);
                callStack.push(calleeCallee);
            }
        }
    }
}


std::unordered_set<procedure> CallStore::getCallChildren(procedure p) {
    return this -> callTable[p];
}

std::unordered_set<procedure> CallStore::getCallParents(procedure p) {
    return this -> callTableReverse[p];
}

std::unordered_set<procedure> CallStore::getCallStarChildren(procedure p) {
    return this -> callTableStar[p];
}

std::unordered_set<procedure> CallStore::getCallStarParents(procedure p) {
    return this -> callTableStarReverse[p];
}
