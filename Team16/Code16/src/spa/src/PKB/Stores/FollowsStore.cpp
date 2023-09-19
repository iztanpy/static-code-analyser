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

bool FollowsStore::isFollow(statementNumber statement1, statementNumber statement2) {
    if (this->FollowsMap.find(statement1) != this->FollowsMap.end()) {
        if (this->FollowsMap[statement1] == statement2) {
            return true;
        }
    }
    return false;
}

// true if statement1 has any follower
bool FollowsStore::isFollow(statementNumber statement1, Wildcard wildcard) {
    if (this->getFollowers(statement1).size() > 0) {
        return true;
    }
    return false;
}


// true if statement1 follows any statement
bool FollowsStore::isFollow(Wildcard wildcard, statementNumber statement1) {
    if (this->getLeader(statement1)) {
        return true;
    }
    return false;
}


bool FollowsStore::isFollow(Wildcard wildcard, Wildcard wildcard2) {
    if (this->FollowsMap.size() > 0) {
        return true;
    }
    return false;
}

FollowsStore::statementNumber FollowsStore::getLeader(statementNumber statement) {
    statementNumber leader = this->FollowsMapReverse[statement];
    return leader;
}

std::unordered_set<FollowsStore::statementNumber> FollowsStore::getFollowers(statementNumber statement) {
    std::unordered_set<statementNumber> followers;
    while (this->FollowsMap.find(statement) != this->FollowsMap.end()) {
        followers.insert(this->FollowsMap[statement]);
        statement = FollowsMap[statement];
    }
    return followers;
}

bool FollowsStore::isFollows(statementNumber statement1, statementNumber statement2) {
    while (this->FollowsMap.find(statement1) != this->FollowsMap.end()) {
        if (this->FollowsMap[statement1] == statement2) {
            return true;
        }
        statement1 = FollowsMap[statement1];
    }
    return false;
}


std::unordered_set<FollowsStore::statementNumber> FollowsStore::getLeaders(statementNumber statement) {
    std::unordered_set<statementNumber> leaders;
    while (this->FollowsMapReverse.find(statement) != this->FollowsMapReverse.end()) {
        leaders.insert(this->FollowsMapReverse[statement]);
        statement = FollowsMapReverse[statement];
    }
    return leaders;
}
