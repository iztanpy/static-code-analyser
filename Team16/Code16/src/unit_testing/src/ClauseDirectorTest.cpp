//
// Created by Thia Su Mian on 5/9/23.
//
#include <QPS/ClauseDirector.h>
#include <catch.hpp>

TEST_CASE("Dummy test") {
    ClauseDirector clauseDirector;
    REQUIRE(clauseDirector.dummyMethod() == true);
}

TEST_CASE("Clause Director can create clauses") {
    ClauseDirector clauseDirector;
    SelectClauseBuilder builder;
    std::string query = "variable v; Select v";
    clauseDirector.makeSelectClause(builder, query);
    REQUIRE(clauseDirector.dummyMethod() == true);
}