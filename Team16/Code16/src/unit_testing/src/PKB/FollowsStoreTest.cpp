#include <stdio.h>
#include <unordered_set>
#include "catch.hpp"
#include "PKB/Stores/FollowsStore.h"
#include "PKB/Helper/Wildcard.h"

typedef int statementNumber;


TEST_CASE("Test Follows Store Add") {
    auto followsStore = FollowsStore();
    followsStore.storeFollows({{1, 2}, {2, 3}, {3, 4}});
    REQUIRE(followsStore.getFollower(1) == 2);
    REQUIRE(followsStore.getFollower(2) == 3);
    REQUIRE(followsStore.getFollower(3) == 4);
}

TEST_CASE("Test Boolean Follows") {
    auto followsStore = FollowsStore();
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
    auto followsStore = FollowsStore();
    followsStore.storeFollows({{1, 2}, {2, 3}, {3, 4}});
    REQUIRE(followsStore.getLeader(2) == 1);
    REQUIRE(followsStore.getLeader(3) == 2);
    REQUIRE(followsStore.getLeader(4) == 3);
}

TEST_CASE("Test Follows Store Get Followers") {
    auto followsStore = FollowsStore();
    followsStore.storeFollows({{1, 2}, {2, 3}, {3, 4}});
    std::unordered_set<statementNumber> followers = followsStore.getFollowers(1);
    REQUIRE(followers.size() == 3);
    REQUIRE(followers.find(2) != followers.end());
    followers = followsStore.getFollowers(2);
    REQUIRE(followers.size() == 2);
    REQUIRE(followers.find(3) != followers.end());
    followers = followsStore.getFollowers(3);
    REQUIRE(followers.size() == 1);
    REQUIRE(followers.find(4) != followers.end());
    followers = followsStore.getFollowers(4);
    REQUIRE(followers.size() == 0);
}

