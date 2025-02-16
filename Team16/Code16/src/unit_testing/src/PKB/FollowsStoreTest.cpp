#include <stdio.h>
#include <unordered_set>
#include "catch.hpp"
#include "PKB/Stores/follows_store.h"
#include "utils/clauses_types.h"

typedef int statementNumber;

TEST_CASE("Test Follows Store Add") {
  auto followsStore = follows_store();
  followsStore.storeFollows({{1, 2}, {2, 3}, {3, 4}});
  REQUIRE(followsStore.getAfter(1) == 2);
  REQUIRE(followsStore.getAfter(2) == 3);
  REQUIRE(followsStore.getAfter(3) == 4);
}

TEST_CASE("Test Boolean Follows") {
  auto followsStore = follows_store();
  followsStore.storeFollows({{1, 2}, {2, 3}, {3, 4}});
  REQUIRE(followsStore.isFollow(1, 2));
  REQUIRE(followsStore.isFollow(2, 3));
  REQUIRE(followsStore.isFollow(3, 4));
  REQUIRE(!followsStore.isFollow(1, 3));
  REQUIRE(!followsStore.isFollow(2, 4));
  REQUIRE(!followsStore.isFollow(1, 4));

  Wildcard wildcard = Wildcard();

  REQUIRE(!followsStore.isFollow(wildcard, 1));

  REQUIRE(followsStore.isFollowStar(1, 2));
  REQUIRE(followsStore.isFollowStar(2, 4));
  REQUIRE(followsStore.isFollowStar(1, 4));
  REQUIRE(followsStore.isFollowStar(1, 2));
  REQUIRE(followsStore.isFollowStar(2, 3));
  REQUIRE(followsStore.isFollowStar(3, 4));
  REQUIRE(!followsStore.isFollowStar(1, 5));
  REQUIRE(!followsStore.isFollowStar(4, 3));
  REQUIRE(!followsStore.isFollowStar(4, 2));
  REQUIRE(!followsStore.isFollowStar(3, 1));
  REQUIRE(!followsStore.isFollowStar(2, 1));

  REQUIRE(!followsStore.isFollowStar(wildcard, 1));
  REQUIRE(followsStore.isFollowStar(wildcard, 2));
  REQUIRE(followsStore.isFollowStar(wildcard, 3));
  REQUIRE(followsStore.isFollowStar(wildcard, 4));

  REQUIRE(followsStore.isFollowStar(1, wildcard));
  REQUIRE(followsStore.isFollowStar(2, wildcard));
  REQUIRE(followsStore.isFollowStar(3, wildcard));
  REQUIRE(!followsStore.isFollowStar(4, wildcard));
}

TEST_CASE("Test Follows Store Reverse") {
  auto followsStore = follows_store();
  followsStore.storeFollows({{1, 2}, {2, 3}, {3, 4}});
  REQUIRE(followsStore.getBefore(2) == 1);
  REQUIRE(followsStore.getBefore(3) == 2);
  REQUIRE(followsStore.getBefore(4) == 3);
}

TEST_CASE("Test Follows Store Get Followers") {
  auto followsStore = follows_store();
  followsStore.storeFollows({{1, 2}, {2, 3}, {3, 4}});
  std::unordered_set<statementNumber> followers = followsStore.getAfters(1);
  REQUIRE(followers.size() == 3);
  REQUIRE(followers.find(2) != followers.end());
  followers = followsStore.getAfters(2);
  REQUIRE(followers.size() == 2);
  REQUIRE(followers.find(3) != followers.end());
  followers = followsStore.getAfters(3);
  REQUIRE(followers.size() == 1);
  REQUIRE(followers.find(4) != followers.end());
  followers = followsStore.getAfters(4);
  REQUIRE(followers.size() == 0);
}

