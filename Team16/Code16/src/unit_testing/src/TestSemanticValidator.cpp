#include "catch.hpp"

#include "qps/clauses/suchthat_clauses/suchthat_clauses_all.h"
#include "qps/clauses/pattern_clauses/pattern_clause.h"
#include "qps/qps_errors/qps_semantic_error.h"
#include "utils/entity_types.h"
#include "qps/clauses/attr_ref.h"
#include "qps/clauses/with_clauses/with_clause.h"

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

TEST_CASE("Affects::Affects", "[Affects]") {
  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref1(Declaration{"x", DesignEntity::PROCEDURE});
    StmtRef stmt_ref2(1);
    REQUIRE_THROWS_AS(Affects(stmt_ref1, stmt_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(Affects(stmt_ref1, stmt_ref2), "Invalid LHS synonym. Must be statements entities");
  }

  SECTION("Constructor throws semantic error") {
    StmtRef stmt_ref1(Wildcard::Value);
    StmtRef stmt_ref2(Declaration{"x", DesignEntity::VARIABLE});
    REQUIRE_THROWS_AS(Affects(stmt_ref1, stmt_ref2), QpsSemanticError);
    REQUIRE_THROWS_WITH(Affects(stmt_ref1, stmt_ref2), "Invalid RHS synonym. Must be statements entities");
  }

  SECTION("Constructor not throwing semantic error") {
    StmtRef stmt_ref1(Wildcard::Value);
    StmtRef stmt_ref2(Declaration{"x", DesignEntity::STMT});
    REQUIRE_NOTHROW(Affects(stmt_ref1, stmt_ref2));
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

TEST_CASE("AttrRef::AttrRef", "[AttrRef]") {
  SECTION("Constructor throws semantic error") {
    // Invalid: VARIABLE should not be paired with PROCNAME
    REQUIRE_THROWS_AS(AttrRef({"v", DesignEntity::VARIABLE}, AttrName::PROCNAME), QpsSemanticError);
    REQUIRE_THROWS_WITH(AttrRef({"v", DesignEntity::VARIABLE}, AttrName::PROCNAME), "Invalid AttrName for synonym");

    // Invalid: PROCEDURE should not be paired with VARNAME
    REQUIRE_THROWS_AS(AttrRef({"p", DesignEntity::PROCEDURE}, AttrName::VARNAME), QpsSemanticError);
    REQUIRE_THROWS_WITH(AttrRef({"p", DesignEntity::PROCEDURE}, AttrName::VARNAME), "Invalid AttrName for synonym");

    // Invalid: CONSTANT should not be paired with PROCNAME
    REQUIRE_THROWS_AS(AttrRef({"c", DesignEntity::CONSTANT}, AttrName::PROCNAME), QpsSemanticError);
    REQUIRE_THROWS_WITH(AttrRef({"c", DesignEntity::CONSTANT}, AttrName::PROCNAME), "Invalid AttrName for synonym");

    // Invalid: READ should not be paired with PROCNAME
    REQUIRE_THROWS_AS(AttrRef({"r", DesignEntity::READ}, AttrName::PROCNAME), QpsSemanticError);
    REQUIRE_THROWS_WITH(AttrRef({"r", DesignEntity::READ}, AttrName::PROCNAME), "Invalid AttrName for synonym");

    // Invalid: PRINT should not be paired with VALUE
    REQUIRE_THROWS_AS(AttrRef({"pr", DesignEntity::PRINT}, AttrName::VALUE), QpsSemanticError);
    REQUIRE_THROWS_WITH(AttrRef({"pr", DesignEntity::PRINT}, AttrName::VALUE), "Invalid AttrName for synonym");

    // Invalid: CALL should not be paired with VARNAME
    REQUIRE_THROWS_AS(AttrRef({"c", DesignEntity::CALL}, AttrName::VARNAME), QpsSemanticError);
    REQUIRE_THROWS_WITH(AttrRef({"c", DesignEntity::CALL}, AttrName::VARNAME), "Invalid AttrName for synonym");

    // Invalid: WHILE_LOOP should not be paired with VALUE
    REQUIRE_THROWS_AS(AttrRef({"w", DesignEntity::WHILE_LOOP}, AttrName::VALUE), QpsSemanticError);
    REQUIRE_THROWS_WITH(AttrRef({"w", DesignEntity::WHILE_LOOP}, AttrName::VALUE), "Invalid AttrName for synonym");

    // Invalid: IF_STMT should not be paired with VARNAME
    REQUIRE_THROWS_AS(AttrRef({"i", DesignEntity::IF_STMT}, AttrName::VARNAME), QpsSemanticError);
    REQUIRE_THROWS_WITH(AttrRef({"i", DesignEntity::IF_STMT}, AttrName::VARNAME), "Invalid AttrName for synonym");

    // Invalid: ASSIGN should not be paired with PROCNAME
    REQUIRE_THROWS_AS(AttrRef({"a", DesignEntity::ASSIGN}, AttrName::PROCNAME), QpsSemanticError);
    REQUIRE_THROWS_WITH(AttrRef({"a", DesignEntity::ASSIGN}, AttrName::PROCNAME), "Invalid AttrName for synonym");

    // Invalid: STMT should not be paired with PROCNAME
    REQUIRE_THROWS_AS(AttrRef({"s", DesignEntity::STMT}, AttrName::PROCNAME), QpsSemanticError);
    REQUIRE_THROWS_WITH(AttrRef({"s", DesignEntity::STMT}, AttrName::PROCNAME), "Invalid AttrName for synonym");

  }

  SECTION("Constructor not throwing semantic error") {
    // procedure.procName, call.procName, variable.varName, read.varName, print.varName: NAME
    REQUIRE_NOTHROW(AttrRef({"p", DesignEntity::PROCEDURE}, AttrName::PROCNAME));
    REQUIRE_NOTHROW(AttrRef({"c", DesignEntity::CALL}, AttrName::PROCNAME));
    REQUIRE_NOTHROW(AttrRef({"v", DesignEntity::VARIABLE}, AttrName::VARNAME));
    REQUIRE_NOTHROW(AttrRef({"r", DesignEntity::READ}, AttrName::VARNAME));
    REQUIRE_NOTHROW(AttrRef({"pr", DesignEntity::PRINT}, AttrName::VARNAME));

    // constant.value: INTEGER
    REQUIRE_NOTHROW(AttrRef({"c", DesignEntity::CONSTANT}, AttrName::VALUE));

    // stmt.stmt#, read.stmt#, print.stmt#, call.stmt#, while.stmt#, if.stmt#, assign.stmt#: INTEGER
    REQUIRE_NOTHROW(AttrRef({"s", DesignEntity::STMT}, AttrName::STMTNUM));
    REQUIRE_NOTHROW(AttrRef({"r", DesignEntity::READ}, AttrName::STMTNUM));
    REQUIRE_NOTHROW(AttrRef({"pr", DesignEntity::PRINT}, AttrName::STMTNUM));
    REQUIRE_NOTHROW(AttrRef({"c", DesignEntity::CALL}, AttrName::STMTNUM));
    REQUIRE_NOTHROW(AttrRef({"w", DesignEntity::WHILE_LOOP}, AttrName::STMTNUM));
    REQUIRE_NOTHROW(AttrRef({"i", DesignEntity::IF_STMT}, AttrName::STMTNUM));
    REQUIRE_NOTHROW(AttrRef({"a", DesignEntity::ASSIGN}, AttrName::STMTNUM));
  }
}

TEST_CASE("WithClause::WithClause", "[WithClause]") {
  SECTION("Constructor throws semantic error") {
    auto message_matcher = Catch::Matchers::Message("LHS and RHS of WithClause must be of the same underlying type");

    // Test 0: Mismatch between Name and INTEGER for procedure name
    REQUIRE_THROWS_MATCHES(
        WithClause(AttrRef({"c", DesignEntity::CALL}, AttrName::PROCNAME), 8),
        QpsSemanticError,
        message_matcher
    );

    // Test 1: Mismatch between Name and INTEGER for statement number
    REQUIRE_THROWS_MATCHES(
        WithClause(AttrRef({"s", DesignEntity::STMT}, AttrName::STMTNUM), std::string("10")),
        QpsSemanticError,
        message_matcher
    );

    // Test 2: Mismatch between Name and INTEGER for procedure name
    REQUIRE_THROWS_MATCHES(
        WithClause(AttrRef({"p", DesignEntity::PROCEDURE}, AttrName::PROCNAME), 123),
        QpsSemanticError,
        message_matcher
    );

    // Test 3: Mismatch between Name and INTEGER for variable name
    REQUIRE_THROWS_MATCHES(
        WithClause(AttrRef({"v", DesignEntity::VARIABLE}, AttrName::VARNAME), 999),
        QpsSemanticError,
        message_matcher
    );

    // Test 4: Mismatch between INTEGER for statement number and Name for read variable
    REQUIRE_THROWS_MATCHES(
        WithClause(AttrRef({"r", DesignEntity::READ}, AttrName::STMTNUM), std::string("someVar")),
        QpsSemanticError,
        message_matcher
    );

    // Test 5: Mismatch between INTEGER for constant value and Name for call procedure
    REQUIRE_THROWS_MATCHES(
        WithClause(AttrRef({"con", DesignEntity::CONSTANT}, AttrName::VALUE),
                   AttrRef({"c", DesignEntity::CALL}, AttrName::PROCNAME)),
        QpsSemanticError,
        message_matcher
    );

    // Test 6: Mismatch between INTEGER and string
    REQUIRE_THROWS_MATCHES(
        WithClause(5, std::string("five")),
        QpsSemanticError,
        message_matcher
    );
  }

  SECTION("Constructor not throwing semantic error") {
    // Test 1: PROCNAME and VARNAME has the same underlying type
    REQUIRE_NOTHROW(
        WithClause(AttrRef({"p", DesignEntity::PROCEDURE}, AttrName::PROCNAME),
                   AttrRef({"v", DesignEntity::VARIABLE}, AttrName::VARNAME))
    );

    // Test 2: STMTNUM and VALUE has the same underlying type
    REQUIRE_NOTHROW(
        WithClause(AttrRef({"c", DesignEntity::CONSTANT}, AttrName::VALUE),
                   AttrRef({"s", DesignEntity::STMT}, AttrName::STMTNUM))
    );

    // Test 3: Mismatch between INTEGER types (read and print)
    REQUIRE_NOTHROW(
        WithClause(AttrRef({"p", DesignEntity::PROCEDURE}, AttrName::PROCNAME),
                   AttrRef({"c", DesignEntity::CALL}, AttrName::PROCNAME))
    );

    // Test 4: Mismatch between INTEGER types (read and print)
    REQUIRE_NOTHROW(
        WithClause(AttrRef({"pt", DesignEntity::PRINT}, AttrName::VARNAME),
                   AttrRef({"v", DesignEntity::VARIABLE}, AttrName::VARNAME))
    );
    REQUIRE_NOTHROW(WithClause(7, 10));
    REQUIRE_NOTHROW(WithClause("procedureName", "varName"));

    // Matching Name types for procedure name with another procedure name
    REQUIRE_NOTHROW(
        WithClause(AttrRef({"p1", DesignEntity::PROCEDURE}, AttrName::PROCNAME),
                   AttrRef({"p2", DesignEntity::PROCEDURE}, AttrName::PROCNAME))
    );

    // Matching Name types for variable name with a string
    REQUIRE_NOTHROW(
        WithClause(AttrRef({"v", DesignEntity::VARIABLE}, AttrName::VARNAME), std::string("variableName"))
    );

    // Matching Name types for call procedure name with read variable name
    REQUIRE_NOTHROW(
        WithClause(AttrRef({"c", DesignEntity::CALL}, AttrName::PROCNAME),
                   AttrRef({"r", DesignEntity::READ}, AttrName::VARNAME))
    );

    // Matching Name types for print variable name with a string
    REQUIRE_NOTHROW(
        WithClause("printVar", AttrRef({"pr", DesignEntity::PRINT}, AttrName::VARNAME))
    );

    // Matching INTEGER types for statement number with integer
    REQUIRE_NOTHROW(
        WithClause(AttrRef({"s", DesignEntity::STMT}, AttrName::STMTNUM), 5)
    );

    // Matching INTEGER types for constant value with integer
    REQUIRE_NOTHROW(
        WithClause(10, AttrRef({"c", DesignEntity::CONSTANT}, AttrName::VALUE))
    );

    // Matching INTEGER types for assign statement number with read statement number
    REQUIRE_NOTHROW(
        WithClause(AttrRef({"a", DesignEntity::ASSIGN}, AttrName::STMTNUM),
                   AttrRef({"rd", DesignEntity::READ}, AttrName::STMTNUM))
    );

    // Matching Name types for call procedure name with a string
    REQUIRE_NOTHROW(
        WithClause(AttrRef({"cal", DesignEntity::CALL}, AttrName::PROCNAME), std::string("someProcedure"))
    );

    // Matching INTEGER types for while statement number with integer
    REQUIRE_NOTHROW(
        WithClause(20, AttrRef({"w", DesignEntity::WHILE_LOOP}, AttrName::STMTNUM))
    );

    // Matching Name types for read variable name with print variable name
    REQUIRE_NOTHROW(
        WithClause(AttrRef({"r", DesignEntity::READ}, AttrName::VARNAME),
                   AttrRef({"pt", DesignEntity::PRINT}, AttrName::VARNAME))
    );

  }
}