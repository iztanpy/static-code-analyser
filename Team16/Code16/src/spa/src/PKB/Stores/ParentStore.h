#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "../Helper/StmtEntity.h"
#include "../Helper/Wildcard.h"


class ParentStore {
 private:
    typedef std::string variable;
    typedef int statementNumber;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> ParentMap;
    std::unordered_map<statementNumber, statementNumber> ParentMapReverse;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> ParentStarMap;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> ParentStarMapReverse;

 public:
    ParentStore();

    void storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map);

    bool isParent(statementNumber parent, statementNumber child);

    bool isParent(statementNumber parent, Wildcard wildcard);

    bool isParent(Wildcard wildcard, statementNumber child);

    bool isParent(Wildcard wildcard, Wildcard wildcard2);

    std::unordered_set<statementNumber> getChildren(statementNumber statement);

    statementNumber getParent(statementNumber statement);

    bool isParentStar(statementNumber parent, statementNumber child);

    bool isParentStar(statementNumber parent, Wildcard wildcard);

    bool isParentStar(Wildcard wildcard, statementNumber child);

    bool isParentStar(Wildcard wildcard, Wildcard wildcard2);

    std::unordered_set<statementNumber> getChildrens(statementNumber statement);

    std::unordered_set<statementNumber> getParents(statementNumber statement);
};
