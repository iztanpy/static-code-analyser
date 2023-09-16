//
// Created by Isaac Tan on 14/9/23.
//
#include <stdio.h>
#include <unordered_set>
#include "catch.hpp"
#include "PKB/Stores/FollowsStore.h"

typedef int statementNumber;


TEST_CASE("Test Follows Store Add") {
    auto followsStore = FollowsStore();
    followsStore.storeFollows({{1, 2}, {2, 3}, {3, 4}});
    REQUIRE(followsStore.getFollower(1) == 2);
    REQUIRE(followsStore.getFollower(2) == 3);
    REQUIRE(followsStore.getFollower(3) == 4);
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

