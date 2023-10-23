#include "catch.hpp"

#include "qps/clauses/suchthat_clauses/suchthat_clauses_all.h"
#include "qps/clauses/pattern_clauses/pattern_clause.h"
#include "qps/qps_errors/qps_semantic_error.h"
#include "utils/entity_types.h"

TEST_CASE("Uses::Uses", "[Uses]") {
  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref(Wildcard::Value);
    EntRef ent_ref(Declaration{"x", DesignEntity::VARIABLE});
    REQUIRE_THROWS_AS(UsesS(stmt_ref, ent_ref), QpsSemanticError);
    REQUIRE_THROWS_WITH(UsesS(stmt_ref, ent_ref), "[Uses/Modifies] LHS cannot be wildcard");
  }

  SECTION("Constructor throws semantic error") {
    EntRef ent_ref1("a");
    EntRef ent_ref2(Declaration{"x", DesignEntity::PROCEDURE});
    REQUIRE_THROWS_AS(UsesP(ent_ref1, ent_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(UsesP(ent_ref1, ent_ref2), "[Uses/Modifies] Invalid RHS synonym");
  }

  SECTION("Constructor not throwing semantic error") {
    StmtRef ent_ref1(Declaration{"x", DesignEntity::READ});
    EntRef ent_ref2("y");
    REQUIRE_NOTHROW(UsesS(ent_ref1, ent_ref2));
  }

  SECTION("Constructor not throws semantic error") {
    EntRef ent_ref1(Declaration{"x", DesignEntity::PROCEDURE});
    EntRef ent_ref2(Wildcard::Value);
    REQUIRE_NOTHROW(UsesP(ent_ref1, ent_ref2));
  }
}

TEST_CASE("Modifies::Modifies", "[Modifies]") {
  SECTION("Constructor not throwing semantic error") {
    StmtRef stmt_ref(Declaration{"x", DesignEntity::PRINT});
    EntRef ent_ref("y");
    REQUIRE_NOTHROW(ModifiesS(stmt_ref, ent_ref));
  }

  SECTION("Constructor not throwing semantic error") {
    EntRef ent_ref1(Declaration{"x", DesignEntity::PROCEDURE});
    EntRef ent_ref2("y");
    REQUIRE_NOTHROW(ModifiesP(ent_ref1, ent_ref2));
  }

  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref(1);
    EntRef ent_ref(Declaration{"x", DesignEntity::PRINT});
    REQUIRE_THROWS_AS(ModifiesS(stmt_ref, ent_ref), QpsSemanticError);
    REQUIRE_THROWS_WITH(ModifiesS(stmt_ref, ent_ref), "[Uses/Modifies] Invalid RHS synonym");
  }

  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref(1);
    EntRef ent_ref(Declaration{"x", DesignEntity::PROCEDURE});
    REQUIRE_THROWS_AS(ModifiesS(stmt_ref, ent_ref), QpsSemanticError);
    REQUIRE_THROWS_WITH(ModifiesS(stmt_ref, ent_ref), "[Uses/Modifies] Invalid RHS synonym");
  }

  SECTION("Constructor throws semantic error") {
    EntRef ent_ref1("main");
    EntRef ent_ref2(Declaration{"x", DesignEntity::PROCEDURE});
    REQUIRE_THROWS_AS(ModifiesP(ent_ref1, ent_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(ModifiesP(ent_ref1, ent_ref2), "[Uses/Modifies] Invalid RHS synonym");
  }
}

TEST_CASE("Parent::Parent", "[Parent]") {
  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref1(Declaration{"x", DesignEntity::PROCEDURE});
    StmtRef stmt_ref2(1);
    REQUIRE_THROWS_AS(Parent(stmt_ref1, stmt_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(Parent(stmt_ref1, stmt_ref2), "Invalid LHS synonym. Must be statements entities");
  }

  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref1(Wildcard::Value);
    StmtRef stmt_ref2(Declaration{"x", DesignEntity::VARIABLE});
    REQUIRE_THROWS_AS(ParentT(stmt_ref1, stmt_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(ParentT(stmt_ref1, stmt_ref2), "Invalid RHS synonym. Must be statements entities");
  }
}

TEST_CASE("Follows::Follows", "[Follows]") {
  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref1(Declaration{"x", DesignEntity::VARIABLE});
    StmtRef stmt_ref2(1);
    REQUIRE_THROWS_AS(Follows(stmt_ref1, stmt_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(Follows(stmt_ref1, stmt_ref2), "Invalid LHS synonym. Must be statements entities");
  }

  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref1(Wildcard::Value);
    StmtRef stmt_ref2(Declaration{"x", DesignEntity::CONSTANT});
    REQUIRE_THROWS_AS(FollowsT(stmt_ref1, stmt_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(FollowsT(stmt_ref1, stmt_ref2), "Invalid RHS synonym. Must be statements entities");
  }
}

TEST_CASE("Calls::Calls", "[Calls]") {
  SECTION("Constructor throws semantic error") {
    EntRef ent_ref1("a");
    EntRef ent_ref2(Declaration{"a", DesignEntity::VARIABLE});
    REQUIRE_THROWS_AS(Calls(ent_ref1, ent_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(Calls(ent_ref1, ent_ref2), "[Calls] Invalid RHS synonym");
  }

  SECTION("Constructor throws semantic error") {
    EntRef ent_ref1(Declaration{"a", DesignEntity::ASSIGN});
    EntRef ent_ref2("a");
    REQUIRE_THROWS_AS(Calls(ent_ref1, ent_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(Calls(ent_ref1, ent_ref2), "[Calls] Invalid LHS synonym");
  }

  SECTION("Constructor not throwing semantic error") {
    EntRef ent_ref1(Declaration{"x", DesignEntity::PROCEDURE});
    EntRef ent_ref2(Declaration{"y", DesignEntity::PROCEDURE});
    REQUIRE_NOTHROW(Calls(ent_ref1, ent_ref2));
  }
}

TEST_CASE("CallsT::CallsT", "[CallsT]") {
  SECTION("Constructor throws semantic error") {
    EntRef ent_ref1("a");
    EntRef ent_ref2(Declaration{"a", DesignEntity::STMT});
    REQUIRE_THROWS_AS(CallsT(ent_ref1, ent_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(CallsT(ent_ref1, ent_ref2), "[Calls] Invalid RHS synonym");
  }

  SECTION("Constructor throws semantic error") {
    EntRef ent_ref1(Declaration{"a", DesignEntity::PROCEDURE});
    EntRef ent_ref2(Declaration{"a", DesignEntity::READ});
    REQUIRE_THROWS_AS(CallsT(ent_ref1, ent_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(CallsT(ent_ref1, ent_ref2), "[Calls] Invalid RHS synonym");
  }

  SECTION("Constructor not throwing semantic error") {
    EntRef ent_ref1(Wildcard::Value);
    EntRef ent_ref2(Declaration{"x", DesignEntity::PROCEDURE});
    REQUIRE_NOTHROW(CallsT(ent_ref1, ent_ref2));
  }
}

TEST_CASE("AssignPattern::AssignPattern", "[AssignPattern]") {
  SECTION("Constructor throws semantic error") {
    Declaration syn({"x", DesignEntity::STMT});
    EntRef ent_ref("a");
    PartialExpr expr_spec({"x"});
    REQUIRE_THROWS_AS(AssignPattern(syn, ent_ref, expr_spec), QpsSemanticError);
    REQUIRE_THROWS_WITH(AssignPattern(syn, ent_ref, expr_spec), "[AssignPattern] syn-assign is not an assign synonym");
  }

  SECTION("Constructor throws semantic error") {
    Declaration syn({"x", DesignEntity::ASSIGN});
    EntRef ent_ref(Declaration{"a", DesignEntity::ASSIGN});
    Wildcard expr_spec(Wildcard::Value);
    REQUIRE_THROWS_AS(AssignPattern(syn, ent_ref, expr_spec), QpsSemanticError);
    REQUIRE_THROWS_WITH(AssignPattern(syn, ent_ref, expr_spec),
                        "[Pattern] Invalid LHS synonym. Must be VARIABLE synonym");
  }

  SECTION("Constructor not throwing semantic error") {
    Declaration syn({"x", DesignEntity::ASSIGN});
    EntRef ent_ref(Declaration{"a", DesignEntity::VARIABLE});
    ExactExpr expr_spec({"x"});
    REQUIRE_NOTHROW(AssignPattern(syn, ent_ref, expr_spec));
  }
}

TEST_CASE("Next::Next", "[Next]") {
  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref1(Declaration{"x", DesignEntity::PROCEDURE});
    StmtRef stmt_ref2(1);
    REQUIRE_THROWS_AS(Next(stmt_ref1, stmt_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(Next(stmt_ref1, stmt_ref2), "Invalid LHS synonym. Must be statements entities");
  }

  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref1(Wildcard::Value);
    StmtRef stmt_ref2(Declaration{"x", DesignEntity::VARIABLE});
    REQUIRE_THROWS_AS(NextT(stmt_ref1, stmt_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(NextT(stmt_ref1, stmt_ref2), "Invalid RHS synonym. Must be statements entities");
  }

  SECTION("Constructor not throwing semantic error") {
    StmtRef stmt_ref1(Wildcard::Value);
    StmtRef stmt_ref2(Declaration{"x", DesignEntity::STMT});
    REQUIRE_NOTHROW(NextT(stmt_ref1, stmt_ref2));
  }
}
TEST_CASE("IfPattern::IfPattern", "[IfPattern]") {
  SECTION("Constructor throws semantic error") {
    Declaration syn({"x", DesignEntity::READ});
    EntRef ent_ref("a");
    REQUIRE_THROWS_AS(IfPattern(syn, ent_ref), QpsSemanticError);
    REQUIRE_THROWS_WITH(IfPattern(syn, ent_ref), "[IfPattern] syn-if is not an if synonym");
  }

  SECTION("Constructor throws semantic error") {
    Declaration syn({"x", DesignEntity::IF_STMT});
    EntRef ent_ref(Declaration{"a", DesignEntity::PRINT});
    REQUIRE_THROWS_AS(IfPattern(syn, ent_ref), QpsSemanticError);
    REQUIRE_THROWS_WITH(IfPattern(syn, ent_ref), "[Pattern] Invalid LHS synonym. Must be VARIABLE synonym");
  }

  SECTION("Constructor not throwing semantic error") {
    Declaration syn({"x", DesignEntity::IF_STMT});
    EntRef ent_ref(Wildcard::Value);
    REQUIRE_NOTHROW(IfPattern(syn, ent_ref));
  }
}

TEST_CASE("WhilePattern::WhilePattern", "[WhilePattern]") {
  SECTION("Constructor throws semantic error") {
    Declaration syn({"x", DesignEntity::READ});
    EntRef ent_ref("a");
    REQUIRE_THROWS_AS(WhilePattern(syn, ent_ref), QpsSemanticError);
    REQUIRE_THROWS_WITH(WhilePattern(syn, ent_ref), "[WhilePattern] syn-while is not a while synonym");
  }

  SECTION("Constructor throws semantic error") {
    Declaration syn({"x", DesignEntity::WHILE_LOOP});
    EntRef ent_ref(Declaration{"a", DesignEntity::CALL});
    REQUIRE_THROWS_AS(WhilePattern(syn, ent_ref), QpsSemanticError);
    REQUIRE_THROWS_WITH(WhilePattern(syn, ent_ref), "[Pattern] Invalid LHS synonym. Must be VARIABLE synonym");
  }

  SECTION("Constructor not throwing semantic error") {
    Declaration syn({"x", DesignEntity::WHILE_LOOP});
    EntRef ent_ref("xyr");
    REQUIRE_NOTHROW(WhilePattern(syn, ent_ref));
  }
}