#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "utils/StmtEntity.h"
#include "utils/clauses_types.h"
#include "./StatementStore.h"


class FollowsStore {
 private:
    typedef int statementNumber;
    typedef std::string variable;
    typedef std::string stmtentity;
    std::unordered_map<statementNumber, statementNumber> FollowMap;
    std::unordered_map<statementNumber, statementNumber> FollowMapReverse;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> FollowStarMap;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> FollowStarMapReverse;

 public:
    FollowsStore();

    void storeFollows(std::unordered_map<statementNumber, statementNumber> map);

    // Follow methods

    statementNumber getFollower(statementNumber statement);

    bool isFollow(statementNumber statement1, statementNumber statement2);

    bool isFollow(statementNumber statement1, Wildcard wildcard);

    bool isFollow(Wildcard wildcard, statementNumber statement1);

    bool isFollow(Wildcard wildcard, Wildcard wildcard2);

    statementNumber getLeader(statementNumber statement);

    // Follow* methods

    bool isFollowStar(statementNumber statement1, statementNumber statement2);

    bool isFollowStar(statementNumber statement1, Wildcard wildcard);

    bool isFollowStar(Wildcard wildcard, statementNumber statement1);

    bool isFollowStar(Wildcard wildcard, Wildcard wildcard2);

    std::unordered_set<statementNumber> getFollowers(statementNumber statement);

    std::unordered_set<statementNumber> getLeaders(statementNumber statement);
};
