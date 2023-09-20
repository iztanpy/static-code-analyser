#include "FollowsStore.h"
#include <iostream>


FollowsStore::FollowsStore() {
    std::unordered_map<statementNumber, statementNumber> FollowMap;
    std::unordered_map<statementNumber, statementNumber> FollowsMapReverse;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> FollowStarMap;
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> FollowStarMapReverse;
}

void FollowsStore::storeFollows(std::unordered_map<statementNumber, statementNumber> map) {
    this->FollowMap = map;
    for (auto const& x : map) {
        this->FollowMapReverse[x.second] = x.first;
    }

    for (auto const& x : map) {
		std::unordered_set<statementNumber> followers;
		statementNumber statement = x.first;
		while (this->FollowMap.find(statement) != this->FollowMap.end()) {
			followers.insert(this->FollowMap[statement]);
			statement = FollowMap[statement];
		}
        if (followers.size() > 0) {
			this->FollowStarMap[x.first] = followers;
        }
	}

    for (auto const& x : map) {
        std::unordered_set<statementNumber> leaders;
        statementNumber statement = x.second;
        while (this->FollowMapReverse.find(statement) != this->FollowMapReverse.end()) {
			leaders.insert(this->FollowMapReverse[statement]);
			statement = FollowMapReverse[statement];
		}
        if (leaders.size() > 0) {
            this->FollowStarMapReverse[x.second] = leaders;
        }
    }
}
FollowsStore::statementNumber FollowsStore::getFollower(statementNumber statement) {
    statementNumber follower = this->FollowMap[statement];
    return follower;
}

bool FollowsStore::isFollow(statementNumber statement1, statementNumber statement2) {
    if (this->FollowMap.find(statement1) != this->FollowMap.end()) {
        if (this->FollowMap[statement1] == statement2) {
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
    if (this->FollowMap.size() > 0) {
        return true;
    }
    return false;
}

FollowsStore::statementNumber FollowsStore::getLeader(statementNumber statement) {
    statementNumber leader = this->FollowMapReverse[statement];
    return leader;
}


bool FollowsStore::isFollowStar(statementNumber statement1, statementNumber statement2) {
    for (auto const& x : this->FollowStarMap[statement1]) {
        if (x == statement2) {
			return true;
		}
	}
    return false;
}

bool FollowsStore::isFollowStar(statementNumber statement1, Wildcard wildcard) {
    if (this->FollowStarMap[statement1].size() > 0) {
		return true;
	}
    return false;
}

bool FollowsStore::isFollowStar(Wildcard wildcard, statementNumber statement1) {
    if (this->FollowStarMapReverse[statement1].size() > 0) {
		return true;
	}
    return false;
}

bool FollowsStore::isFollowStar(Wildcard wildcard, Wildcard wildcard2) {
	if (this->FollowStarMap.size() > 0) {
		return true;
	}
	return false;
}


std::unordered_set<FollowsStore::statementNumber> FollowsStore::getFollowers(statementNumber statement) {
    return this->FollowStarMap[statement];
}

std::unordered_set<FollowsStore::statementNumber> FollowsStore::getLeaders(statementNumber statement) {
    return this->FollowStarMapReverse[statement];
}
