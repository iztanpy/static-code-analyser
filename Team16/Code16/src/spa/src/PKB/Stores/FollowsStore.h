#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>


class FollowsStore {
 private:
    typedef int statementNumber;
    std::unordered_map<statementNumber, statementNumber> FollowsMap;
    std::unordered_map<statementNumber, statementNumber> FollowsMapReverse;

 public:
    FollowsStore();

    void storeFollows(std::unordered_map<statementNumber, statementNumber> map);

    statementNumber getFollower(statementNumber statement);

    bool FollowsStore::isFollows(statementNumber statement1, statementNumber statement2);

    statementNumber getLeader(statementNumber statement);

    std::unordered_set<statementNumber> getFollowers(statementNumber statement);

    std::unordered_set<statementNumber> getLeaders(statementNumber statement);
};
