#include "PKB/Stores/follows_store.h"

follows_store::follows_store() {
  std::unordered_map < statementNumber, statementNumber > FollowMap;
  std::unordered_map < statementNumber, statementNumber > FollowMapReverse;
  std::unordered_map<statementNumber, std::unordered_set<statementNumber>> FollowStarMap;
  std::unordered_map<statementNumber, std::unordered_set<statementNumber>> FollowStarMapReverse;
}

void follows_store::storeFollows(std::unordered_map<statementNumber, statementNumber> map) {
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
follows_store::statementNumber follows_store::getAfter(statementNumber statement) {
  statementNumber after = this->FollowMap[statement];
  return after;
}

bool follows_store::isFollow(statementNumber statement1, statementNumber statement2) {
  if (this->FollowMap.find(statement1) != this->FollowMap.end()) {
    if (this->FollowMap[statement1] == statement2) {
      return true;
    }
  }
  return false;
}

// true if statement1 has any follower
bool follows_store::isFollow(statementNumber statement1, Wildcard wildcard) {
  if (this->getAfters(statement1).size() > 0) {
    return true;
  }
  return false;
}

// true if statement1 follows any statement
bool follows_store::isFollow(Wildcard wildcard, statementNumber statement1) {
  if (this->getBefore(statement1)) {
    return true;
  }
  return false;
}

bool follows_store::isFollow(Wildcard wildcard, Wildcard wildcard2) {
  if (this->FollowMap.size() > 0) {
    return true;
  }
  return false;
}

follows_store::statementNumber follows_store::getBefore(statementNumber statement) {
  statementNumber before = this->FollowMapReverse[statement];
  return before;
}

bool follows_store::isFollowStar(statementNumber statement1, statementNumber statement2) {
  for (auto const& x : this->FollowStarMap[statement1]) {
    if (x == statement2) {
      return true;
    }
  }
  return false;
}

bool follows_store::isFollowStar(statementNumber statement1, Wildcard wildcard) {
  if (this->FollowStarMap[statement1].size() > 0) {
    return true;
  }
  return false;
}

bool follows_store::isFollowStar(Wildcard wildcard, statementNumber statement1) {
  if (this->FollowStarMapReverse[statement1].size() > 0) {
    return true;
  }
  return false;
}

bool follows_store::isFollowStar(Wildcard wildcard, Wildcard wildcard2) {
  if (this->FollowStarMap.size() > 0) {
    return true;
  }
  return false;
}

std::unordered_set<follows_store::statementNumber> follows_store::getAfters(statementNumber statement) {
  return this->FollowStarMap[statement];
}

std::unordered_set<follows_store::statementNumber> follows_store::getBefores(statementNumber statement) {
  return this->FollowStarMapReverse[statement];
}
