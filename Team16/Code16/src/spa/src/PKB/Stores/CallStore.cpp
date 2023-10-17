#include "CallStore.h"
#include <stack>
#include <iostream>
#include "utils/Error.h"

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
                if (caller == calleeCallee) {
                    throw InvalidSemanticError();
                }
                this->callTableStar[caller].insert(calleeCallee);
                this->callTableStarReverse[calleeCallee].insert(caller);
                callStack.push(calleeCallee);
            }
        }
    }
}

std::unordered_set<procedure> CallStore::getCallChildren(procedure p) {
    return this->callTable[p];
}

std::unordered_set<procedure> CallStore::call(StmtEntity proc, Wildcard wildcard) {
    std::unordered_set<procedure> result;
    for (auto const& x : callTable) {
        if (x.second.size() > 0) {
            result.insert(x.first);
        }
    }
    return result;
}

std::unordered_set<procedure> CallStore::call(Wildcard wildcard, StmtEntity proc) {
    std::unordered_set<procedure> result;
    for (auto const& x : callTableReverse) {
        if (x.second.size() > 0) {
            result.insert(x.first);
        }
    }
    return result;
}

bool CallStore::isCall(Wildcard wildcard, Wildcard wildcard2) {
    return !callTable.empty();
}

bool CallStore::isCall(Wildcard wildcard, procedure p) {
    return callTableReverse[p] != std::unordered_set<procedure>();
}

bool CallStore::isCall(procedure p, Wildcard wildcard) {
    return callTable[p] != std::unordered_set<procedure>();
}

bool CallStore::isCall(procedure p, procedure p2) {
    return callTable[p].find(p2) != callTable[p].end();
}

std::unordered_map<procedure, std::unordered_set<procedure>> CallStore::getCallStar() {
    return this->callTableStar;
}

std::unordered_set<std::pair<procedure, procedure>, PairHash> CallStore::call(StmtEntity proc1,
    StmtEntity proc2) {
    std::unordered_set<std::pair<procedure, procedure>, PairHash> pairs;
    for (auto const& x : callTable) {
        for (auto const& second : x.second) {
            pairs.insert(std::make_pair(x.first, second));
        }
    }
    return pairs;
}

std::unordered_set<procedure> CallStore::getCallParents(procedure p) {
    return this->callTableReverse[p];
}

std::unordered_set<std::pair<procedure, procedure>, PairHash> CallStore::callStar(StmtEntity proc1,
    StmtEntity proc2) {
    std::unordered_set<std::pair<procedure, procedure>, PairHash> pairs;
    for (auto const& x : callTableStar) {
        for (auto const& second : x.second) {
            pairs.insert(std::make_pair(x.first, second));
        }
    }
    return pairs;
}

std::unordered_set<procedure> CallStore::callStar(StmtEntity proc, Wildcard wildcard) {
    std::unordered_set<procedure> result;
    for (auto const& x : callTableStar) {
        if (x.second.size() > 0) {
            result.insert(x.first);
        }
    }
    return result;
}

std::unordered_set<procedure> CallStore::callStar(Wildcard wildcard, StmtEntity proc) {
    std::unordered_set<procedure> result;
    for (auto const& x : callTableStarReverse) {
        if (x.second.size() > 0) {
            result.insert(x.first);
        }
    }
    return result;
}

std::unordered_set<procedure> CallStore::getCallStarChildren(procedure p) {
    return this->callTableStar[p];
}

std::unordered_set<procedure> CallStore::getCallStarParents(procedure p) {
    return this->callTableStarReverse[p];
}

bool CallStore::isCallStar(Wildcard wildcard, Wildcard wildcard2) {
    return !callTableStar.empty();
}

bool CallStore::isCallStar(Wildcard wildcard, procedure p) {
    return callTableStarReverse[p] != std::unordered_set<procedure>();
}

bool CallStore::isCallStar(procedure p, Wildcard wildcard) {
    return callTableStar[p] != std::unordered_set<procedure>();
}

bool CallStore::isCallStar(procedure p, procedure p2) {
    return callTableStar[p].find(p2) != callTableStar[p].end();
}
