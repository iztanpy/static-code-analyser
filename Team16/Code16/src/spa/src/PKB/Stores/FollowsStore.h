#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "../Helper/StmtEntity.h"
#include "../Helper/Wildcard.h"
#include "./StatementStore.h"


class FollowsStore {
 private:
    typedef int statementNumber;
    typedef std::string variable;
    typedef std::string stmtentity;
    std::unordered_map<statementNumber, statementNumber> FollowsMap;
    std::unordered_map<statementNumber, statementNumber> FollowsMapReverse;

 public:
    FollowsStore();

    void storeFollows(std::unordered_map<statementNumber, statementNumber> map);

    statementNumber getFollower(statementNumber statement);

    bool isFollow(statementNumber statement1, statementNumber statement2);

    bool isFollow(statementNumber statement1, Wildcard wildcard);

    bool isFollow(Wildcard wildcard, statementNumber statement1);

    bool isFollow(Wildcard wildcard, Wildcard wildcard2);

    statementNumber getLeader(statementNumber statement);

    // follows star

    std::unordered_set<statementNumber> getFollowers(statementNumber statement);

    bool isFollows(statementNumber statement1, statementNumber statement2);

    std::unordered_set<statementNumber> getLeaders(statementNumber statement);
};
