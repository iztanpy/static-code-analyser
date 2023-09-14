#include <catch.hpp>
#include "qps/query_parser/clause_builder/clause_director.h"

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
  QueryToken token = {"v", PQLTokenType::SYNONYM};

  std::vector<Declaration> declarations;
  declarations.push_back({"v", DesignEntity::VARIABLE});
  SelectClause selectClause = ClauseDirector::makeSelectClause(builder, token, declarations);
  REQUIRE(selectClause.synonym == "v");
  REQUIRE(selectClause.design_entity == DesignEntity::VARIABLE);
}

TEST_CASE("Such that Clause Builder successfully sets attributes of clause") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> params = {
      {"Parent", PQLTokenType::RELREF},
      {"a", PQLTokenType::SYNONYM},
      {"7", PQLTokenType::INTEGER}
  };

  std::vector<Declaration> declarations = {
      {"a", DesignEntity::ASSIGN}
  };

  builder.setRelRef(RelRef::fromString(params[0].text));
  builder.setLhs(params[1], declarations);
  builder.setRhs(params[2], declarations);
  std::unique_ptr<SuchThatClause> such_that_clause = builder.getClause();
  auto *clause = dynamic_cast<Parent *>(such_that_clause.get());
  RefParam expectedLhs = StmtRef(declarations[0]);
  RefParam expectedRhs = StmtRef(7);
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE("Clause Director can create such that clause") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> tokens = {
      {"Parent", PQLTokenType::RELREF},
      {"a", PQLTokenType::SYNONYM},
      {"7", PQLTokenType::INTEGER}
  };

  std::vector<Declaration> declarations = {
      {"a", DesignEntity::ASSIGN}
  };

  std::unique_ptr<SuchThatClause> such_that_clause = ClauseDirector::makeSuchThatClause(builder, tokens, declarations);
  auto *clause = dynamic_cast<Parent *>(such_that_clause.get());
  RefParam expectedLhs = StmtRef(declarations[0]);
  RefParam expectedRhs = StmtRef(7);
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->rhs, expectedRhs));
}
