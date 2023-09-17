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

TEST_CASE("Such that Clause Builder successfully sets attributes of parent relation") {
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

TEST_CASE("Clause Director can create such that clause with parent relation") {
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

TEST_CASE("Such that Clause Builder successfully sets attributes of parent* relation") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> params = {
      {"Parent*", PQLTokenType::RELREF},
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
  auto *clause = dynamic_cast<ParentS *>(such_that_clause.get());
  RefParam expectedLhs = StmtRef(declarations[0]);
  RefParam expectedRhs = StmtRef(7);
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE("Clause Director can create such that clause with parent* relation") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> tokens = {
      {"Parent*", PQLTokenType::RELREF},
      {"a", PQLTokenType::SYNONYM},
      {"7", PQLTokenType::INTEGER}
  };

  std::vector<Declaration> declarations = {
      {"a", DesignEntity::ASSIGN}
  };

  std::unique_ptr<SuchThatClause> such_that_clause = ClauseDirector::makeSuchThatClause(builder, tokens, declarations);
  auto *clause = dynamic_cast<ParentS *>(such_that_clause.get());
  RefParam expectedLhs = StmtRef(declarations[0]);
  RefParam expectedRhs = StmtRef(7);
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE("Such that Clause Builder successfully sets attributes of follows relation") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> params = {
      {"Follows", PQLTokenType::RELREF},
      {"7", PQLTokenType::INTEGER},
      {"s", PQLTokenType::SYNONYM},
  };

  std::vector<Declaration> declarations = {
      {"s", DesignEntity::STMT}
  };

  builder.setRelRef(RelRef::fromString(params[0].text));
  builder.setLhs(params[1], declarations);
  builder.setRhs(params[2], declarations);
  std::unique_ptr<SuchThatClause> such_that_clause = builder.getClause();
  auto *clause = dynamic_cast<Follows *>(such_that_clause.get());
  RefParam expectedRhs = StmtRef(declarations[0]);
  RefParam expectedLhs = StmtRef(7);
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE("Clause Director can create such that clause with follows relation") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> tokens = {
      {"Follows", PQLTokenType::RELREF},
      {"7", PQLTokenType::INTEGER},
      {"s", PQLTokenType::SYNONYM},
  };

  std::vector<Declaration> declarations = {
      {"s", DesignEntity::STMT}
  };

  std::unique_ptr<SuchThatClause> such_that_clause = ClauseDirector::makeSuchThatClause(builder, tokens, declarations);
  auto *clause = dynamic_cast<Follows *>(such_that_clause.get());
  RefParam expectedRhs = StmtRef(declarations[0]);
  RefParam expectedLhs = StmtRef(7);
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE("Such that Clause Builder successfully sets attributes of UsesS('stmtRef', 'entRef') relation") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> params = {
      {"Uses", PQLTokenType::RELREF},
      {"7", PQLTokenType::INTEGER},
      {"v", PQLTokenType::SYNONYM},
  };

  std::vector<Declaration> declarations = {
      {"v", DesignEntity::VARIABLE}
  };

  builder.setRelRef(RelRef::fromString(params[0].text));
  builder.setLhs(params[1], declarations);
  builder.setRhs(params[2], declarations);
  std::unique_ptr<SuchThatClause> such_that_clause = builder.getClause();
  auto *clause = dynamic_cast<UsesS *>(such_that_clause.get());
  RefParam expectedRhs = EntRef(declarations[0]);
  RefParam expectedLhs = StmtRef(7);
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE("Clause Director can create such that clause with UsesS('stmtRef', 'entRef') relation") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> tokens = {
      {"Uses", PQLTokenType::RELREF},
      {"7", PQLTokenType::INTEGER},
      {"v", PQLTokenType::SYNONYM},
  };

  std::vector<Declaration> declarations = {
      {"v", DesignEntity::VARIABLE}
  };

  std::unique_ptr<SuchThatClause> such_that_clause = ClauseDirector::makeSuchThatClause(builder, tokens, declarations);
  auto *clause = dynamic_cast<UsesS *>(such_that_clause.get());
  RefParam expectedRhs = EntRef(declarations[0]);
  RefParam expectedLhs = StmtRef(7);
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE("Such that Clause Builder successfully sets attributes of UsesP('entRef', 'entRef') relation") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> params = {
      {"Uses", PQLTokenType::RELREF},
      {"main", PQLTokenType::IDENT},
      {"v", PQLTokenType::SYNONYM},
  };

  std::vector<Declaration> declarations = {
      {"v", DesignEntity::VARIABLE}
  };

  builder.setRelRef(RelRef::fromString(params[0].text));
  builder.setLhs(params[1], declarations);
  builder.setRhs(params[2], declarations);
  std::unique_ptr<SuchThatClause> such_that_clause = builder.getClause();
  auto *clause = dynamic_cast<UsesP *>(such_that_clause.get());
  RefParam expectedRhs = EntRef(declarations[0]);
  RefParam expectedLhs = EntRef("main");
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE("Clause Director can create such that clause with UsesP('entRef', 'entRef') relation") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> tokens = {
      {"Uses", PQLTokenType::RELREF},
      {"main", PQLTokenType::IDENT},
      {"v", PQLTokenType::SYNONYM},
  };

  std::vector<Declaration> declarations = {
      {"v", DesignEntity::VARIABLE}
  };

  std::unique_ptr<SuchThatClause> such_that_clause = ClauseDirector::makeSuchThatClause(builder, tokens, declarations);
  auto *clause = dynamic_cast<UsesP *>(such_that_clause.get());
  RefParam expectedRhs = EntRef(declarations[0]);
  RefParam expectedLhs = EntRef("main");
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE("Such that Clause Builder successfully sets attributes of ModifiesS('stmtRef', 'entRef') relation") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> params = {
      {"Modifies", PQLTokenType::RELREF},
      {"p", PQLTokenType::SYNONYM},
      {"x", PQLTokenType::IDENT},
  };

  std::vector<Declaration> declarations = {
      {"p", DesignEntity::PROCEDURE}
  };

  builder.setRelRef(RelRef::fromString(params[0].text));
  builder.setLhs(params[1], declarations);
  builder.setRhs(params[2], declarations);
  std::unique_ptr<SuchThatClause> such_that_clause = builder.getClause();
  auto *clause = dynamic_cast<ModifiesS *>(such_that_clause.get());
  RefParam expectedLhs = StmtRef(declarations[0]);
  RefParam expectedRhs = EntRef("x");
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE("Clause Director can create such that clause with ModifiesS('stmtRef', 'entRef') relation") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> tokens = {
      {"Modifies", PQLTokenType::RELREF},
      {"p", PQLTokenType::SYNONYM},
      {"x", PQLTokenType::IDENT},
  };

  std::vector<Declaration> declarations = {
      {"p", DesignEntity::PROCEDURE}
  };

  std::unique_ptr<SuchThatClause> such_that_clause = ClauseDirector::makeSuchThatClause(builder, tokens, declarations);
  auto *clause = dynamic_cast<ModifiesS *>(such_that_clause.get());
  RefParam expectedLhs = StmtRef(declarations[0]);
  RefParam expectedRhs = EntRef("x");
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE("Such that Clause Builder successfully sets attributes of ModifiesP('entRef', 'entRef') relation") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> params = {
      {"Modifies", PQLTokenType::RELREF},
      {"y", PQLTokenType::IDENT},
      {"x", PQLTokenType::IDENT},
  };

  std::vector<Declaration> declarations = {
      {"a", DesignEntity::ASSIGN}
  };

  builder.setRelRef(RelRef::fromString(params[0].text));
  builder.setLhs(params[1], declarations);
  builder.setRhs(params[2], declarations);
  std::unique_ptr<SuchThatClause> such_that_clause = builder.getClause();
  auto *clause = dynamic_cast<ModifiesP *>(such_that_clause.get());
  RefParam expectedLhs = EntRef("y");
  RefParam expectedRhs = EntRef("x");
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE("Clause Director can create such that clause with ModifiesS with wildcard") {
  SuchThatClauseBuilder builder;
  std::vector<QueryToken> tokens = {
      {"Modifies", PQLTokenType::RELREF},
      {"p", PQLTokenType::SYNONYM},
      {"_", PQLTokenType::WILDCARD},
  };

  std::vector<Declaration> declarations = {
      {"p", DesignEntity::PROCEDURE}
  };

  std::unique_ptr<SuchThatClause> such_that_clause = ClauseDirector::makeSuchThatClause(builder, tokens, declarations);
  auto *clause = dynamic_cast<ModifiesS *>(such_that_clause.get());
  RefParam expectedLhs = StmtRef(declarations[0]);
  RefParam expectedRhs = EntRef(Wildcard::Value);
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->rhs, expectedRhs));
}
