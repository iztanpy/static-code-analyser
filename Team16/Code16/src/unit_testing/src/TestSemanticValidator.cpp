#include "catch.hpp"

#include "qps/clauses/suchthat_clauses/suchthat_clauses_all.h"
#include "qps/qps_errors/qps_semantic_error.h"

TEST_CASE("Uses::Uses", "[Uses]") {
  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref(Wildcard::Value);
    EntRef ent_ref(Declaration{"x", DesignEntity::VARIABLE});
    REQUIRE_THROWS_AS(UsesS(stmt_ref, ent_ref), QpsSemanticError);
    REQUIRE_THROWS_WITH(UsesS(stmt_ref, ent_ref), "[Uses] LHS cannot be wildcard");
  }

  SECTION("Constructor throws semantic error") {
    EntRef ent_ref1(Declaration{"x", DesignEntity::READ});
    EntRef ent_ref2("y");
    REQUIRE_THROWS_AS(UsesP(ent_ref1, ent_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(UsesP(ent_ref1, ent_ref2), "[Uses] Invalid LHS synonym");
  }
}

TEST_CASE("Modifies::Modifies", "[Modifies]") {
  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref(1);
    EntRef ent_ref(Declaration{"x", DesignEntity::PRINT});
    REQUIRE_THROWS_AS(ModifiesS(stmt_ref, ent_ref), QpsSemanticError);
    REQUIRE_THROWS_WITH(ModifiesS(stmt_ref, ent_ref), "[Modifies] Invalid RHS synonym");
  }

  SECTION("Constructor throws semantic error") {
    EntRef ent_ref1(Wildcard::Value);
    EntRef ent_ref2(Declaration{"x", DesignEntity::PROCEDURE});
    REQUIRE_THROWS_AS(ModifiesP(ent_ref1, ent_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(ModifiesP(ent_ref1, ent_ref2), "[Modifies] LHS cannot be wildcard");
  }

  SECTION("Constructor throws semantic error") {
    EntRef ent_ref1("main");
    EntRef ent_ref2(Declaration{"x", DesignEntity::PROCEDURE});
    REQUIRE_THROWS_AS(ModifiesP(ent_ref1, ent_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(ModifiesP(ent_ref1, ent_ref2), "[Modifies] Invalid RHS synonym");
  }
}

TEST_CASE("Parent::Parent", "[Parent]") {
  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref1(Declaration{"x", DesignEntity::PROCEDURE});
    StmtRef stmt_ref2(1);
    REQUIRE_THROWS_AS(Parent(stmt_ref1, stmt_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(Parent(stmt_ref1, stmt_ref2), "[ParentFollows] Invalid LHS synonym");
  }

  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref1(Wildcard::Value);
    StmtRef stmt_ref2(Declaration{"x", DesignEntity::VARIABLE});
    REQUIRE_THROWS_AS(ParentT(stmt_ref1, stmt_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(ParentT(stmt_ref1, stmt_ref2), "[ParentFollows] Invalid RHS synonym");
  }
}

TEST_CASE("Follows::Follows", "[Follows]") {
  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref1(Declaration{"x", DesignEntity::VARIABLE});
    StmtRef stmt_ref2(1);
    REQUIRE_THROWS_AS(Follows(stmt_ref1, stmt_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(Follows(stmt_ref1, stmt_ref2), "[ParentFollows] Invalid LHS synonym");
  }

  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref1(Wildcard::Value);
    StmtRef stmt_ref2(Declaration{"x", DesignEntity::CONSTANT});
    REQUIRE_THROWS_AS(FollowsT(stmt_ref1, stmt_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(FollowsT(stmt_ref1, stmt_ref2), "[ParentFollows] Invalid RHS synonym");
  }
}
