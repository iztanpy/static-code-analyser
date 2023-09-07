#include <catch.hpp>
#include "qps/clause_director.h"

TEST_CASE("Dummy test") {
    ClauseDirector clauseDirector;
    REQUIRE(clauseDirector.dummyMethod() == true);
}

TEST_CASE("Select Clause Builder successfully sets attributes of clause") {
    SelectClauseBuilder builder;
    builder.setEntity(DesignEntity::VARIABLE);
    builder.setSynonym("v");
    SelectClause select_clause = builder.getClause();
    REQUIRE(select_clause.synonym == "v");
    REQUIRE(select_clause.design_entity == DesignEntity::VARIABLE);
}

TEST_CASE("Clause Director can create select clause") {
    SelectClauseBuilder builder;
    QueryToken token = {"v", TokenType::SYNONYM};
    std::vector<Declaration> declarations;
    declarations.push_back({ "v", DesignEntity::VARIABLE });
    SelectClause selectClause = ClauseDirector::makeSelectClause(builder, token, declarations);
    REQUIRE(selectClause.synonym == "v");
    REQUIRE(selectClause.design_entity == DesignEntity::VARIABLE);
}