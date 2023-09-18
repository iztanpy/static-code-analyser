#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>


class ParentStore {
 private:
    typedef std::string variable;
    typedef int statementNumber;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> ParentMap;
    std::unordered_map<statementNumber, statementNumber> ParentMapReverse;

 public:
    ParentStore();

    void storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map);

    std::unordered_set<statementNumber> getChildren(statementNumber statement);

    statementNumber getParent(statementNumber statement);

    bool isParent(statementNumber parent, statementNumber child);

    std::unordered_set<statementNumber> getChildrens(statementNumber statement);

    std::unordered_set<statementNumber> getParents(statementNumber statement);

    bool isParentStar(statementNumber parent, statementNumber child);
};
