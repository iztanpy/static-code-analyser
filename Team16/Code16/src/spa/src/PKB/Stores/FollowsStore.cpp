//
// Created by Isaac Tan on 14/9/23.
//

#include "FollowsStore.h"

FollowsStore::FollowsStore() {
    std::unordered_map<statementNumber, statementNumber> FollowsMap;
    std::unordered_map<statementNumber, statementNumber> FollowsMapReverse;
}

void FollowsStore::storeFollows(std::unordered_map<statementNumber, statementNumber> map) {
    this->FollowsMap = map;
    for (auto const& x : map) {
        this->FollowsMapReverse[x.second] = x.first;
    }
}
FollowsStore::statementNumber FollowsStore::getFollower(statementNumber statement) {
    statementNumber follower = this->FollowsMap[statement];
    return follower;
}

FollowsStore::statementNumber FollowsStore::getLeader(statementNumber statement) {
    statementNumber leader = this->FollowsMapReverse[statement];
    return leader;
}

std::unordered_set<FollowsStore::statementNumber> FollowsStore::getFollowers(statementNumber statement) {
    std::unordered_set<statementNumber> followers;
    if (this->FollowsMap.find(statement) != this->FollowsMap.end()) {
        followers.insert(this->FollowsMap[statement]);
    }
    return followers;
}

std::unordered_set<FollowsStore::statementNumber> FollowsStore::getLeaders(statementNumber statement) {
    std::unordered_set<statementNumber> leaders;
    if (this->FollowsMapReverse.find(statement) != this->FollowsMapReverse.end()) {
        leaders.insert(this->FollowsMapReverse[statement]);
    }
    return leaders;
}
