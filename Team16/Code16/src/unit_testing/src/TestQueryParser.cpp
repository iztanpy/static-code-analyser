#include <catch.hpp>
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/query_parser/query_parser.h"

TEST_CASE("Query Parser can extract declarations from tokens") {
  std::vector<QueryToken> tokens = {
      {"variable", PQLTokenType::DECLARATION},
      {"v1", PQLTokenType::SYNONYM},
      {"v2", PQLTokenType::SYNONYM},
      {"assign", PQLTokenType::DECLARATION},
      {"a1", PQLTokenType::SYNONYM},
      {"a2", PQLTokenType::SYNONYM},
      {"Select", PQLTokenType::SELECT},
      {"v2", PQLTokenType::SYNONYM}
  };
  std::vector<Declaration> declarations = QueryParser::ExtractDeclarations(tokens);
  REQUIRE(declarations.size() == 4);
  REQUIRE(declarations[0].synonym == "v1");
  REQUIRE(declarations[0].design_entity == DesignEntity::VARIABLE);
  REQUIRE(declarations[1].synonym == "v2");
  REQUIRE(declarations[1].design_entity == DesignEntity::VARIABLE);
  REQUIRE(declarations[2].synonym == "a1");
  REQUIRE(declarations[2].design_entity == DesignEntity::ASSIGN);
  REQUIRE(declarations[3].synonym == "a2");
  REQUIRE(declarations[3].design_entity == DesignEntity::ASSIGN);
}

TEST_CASE("Query Parser can extract select tokens") {
  std::vector<QueryToken> tokens = {
      {"variable", PQLTokenType::DECLARATION},
      {"v", PQLTokenType::SYNONYM},
      {"Select", PQLTokenType::SELECT},
      {"v", PQLTokenType::SYNONYM},
      {"such that", PQLTokenType::SUCH_THAT},
      {"Uses", PQLTokenType::RELREF},
  };

  std::vector<QueryToken> selectTokens = QueryParser::ExtractSelectTokens(tokens);
  REQUIRE(selectTokens.size() == 1);
  REQUIRE(selectTokens[0].text == "v");
  REQUIRE(selectTokens[0].type == PQLTokenType::SYNONYM);
}

TEST_CASE("Query Parser can extract multiple select tokens") {
  std::vector<QueryToken> tokens = {
      {"variable", PQLTokenType::DECLARATION},
      {"v", PQLTokenType::SYNONYM},
      {"assign", PQLTokenType::DECLARATION},
      {"a", PQLTokenType::SYNONYM},
      {"Select", PQLTokenType::SELECT},
      {"v", PQLTokenType::SYNONYM},
      {"a", PQLTokenType::SYNONYM},
  };

  std::vector<QueryToken> selectTokens = QueryParser::ExtractSelectTokens(tokens);
  REQUIRE(selectTokens.size() == 2);
  REQUIRE(selectTokens[0].text == "v");
  REQUIRE(selectTokens[0].type == PQLTokenType::SYNONYM);
  REQUIRE(selectTokens[1].text == "a");
  REQUIRE(selectTokens[1].type == PQLTokenType::SYNONYM);
}

TEST_CASE("Query Parser can extract select clause") {
  std::vector<QueryToken> selectTokens = {
      {"v1", PQLTokenType::SYNONYM},
      {"v2", PQLTokenType::SYNONYM},
  };
  std::vector<Declaration> declarations = {
      {"v1", DesignEntity::VARIABLE},
      {"v2", DesignEntity::VARIABLE}
  };
  std::vector<SelectClause> selectClause = QueryParser::ExtractSelectClauses(selectTokens, declarations);
  REQUIRE(selectClause.size() == 2);
  REQUIRE(selectClause[0].design_entity == DesignEntity::VARIABLE);
  REQUIRE(selectClause[0].synonym == "v1");
}

TEST_CASE("Query Parser can extract such that tokens") {
  std::vector<QueryToken> tokens = {
      {"variable", PQLTokenType::DECLARATION},
      {"v", PQLTokenType::SYNONYM},
      {"assign", PQLTokenType::DECLARATION},
      {"a", PQLTokenType::SYNONYM},
      {"Select", PQLTokenType::SELECT},
      {"v", PQLTokenType::SYNONYM},
      {"such that", PQLTokenType::SUCH_THAT},
      {"Uses", PQLTokenType::RELREF},
      {"a", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM}
  };

  std::vector<QueryToken> suchThatTokens = QueryParser::ExtractSuchThatTokens(tokens);
  REQUIRE(suchThatTokens.size() == 3);
  REQUIRE(suchThatTokens[0].text == "Uses");
  REQUIRE(suchThatTokens[0].type == PQLTokenType::RELREF);
  REQUIRE(suchThatTokens[1].text == "a");
  REQUIRE(suchThatTokens[1].type == PQLTokenType::SYNONYM);
  REQUIRE(suchThatTokens[2].text == "v");
  REQUIRE(suchThatTokens[2].type == PQLTokenType::SYNONYM);
}

TEST_CASE("Query Parser can extract multiple such that tokens") {
  std::string sample_query = "variable v; assign a; stmt s; Select v such that Uses(a, v) Parent(s, a)";
  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(sample_query);

  std::vector<QueryToken> suchThatTokens = QueryParser::ExtractSuchThatTokens(tokens);
  REQUIRE(suchThatTokens.size() == 6);
  REQUIRE(suchThatTokens[0].text == "Uses");
  REQUIRE(suchThatTokens[0].type == PQLTokenType::RELREF);
  REQUIRE(suchThatTokens[1].text == "a");
  REQUIRE(suchThatTokens[1].type == PQLTokenType::SYNONYM);
  REQUIRE(suchThatTokens[2].text == "v");
  REQUIRE(suchThatTokens[2].type == PQLTokenType::SYNONYM);
  REQUIRE(suchThatTokens[3].text == "Parent");
  REQUIRE(suchThatTokens[3].type == PQLTokenType::RELREF);
  REQUIRE(suchThatTokens[4].text == "s");
  REQUIRE(suchThatTokens[4].type == PQLTokenType::SYNONYM);
  REQUIRE(suchThatTokens[5].text == "a");
  REQUIRE(suchThatTokens[5].type == PQLTokenType::SYNONYM);
}

TEST_CASE("Query Parser can extract such that clauses with UsesS('stmtRef', 'entRef') relation") {
  std::string sample_query = "variable v;\n"
                             "Select v such that Uses (14, v)";
  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(sample_query);
  std::vector<Declaration> declarations = QueryParser::ExtractDeclarations(tokens);
  std::vector<QueryToken> such_that_tokens = QueryParser::ExtractSuchThatTokens(tokens);
  std::vector<std::unique_ptr<SuchThatClause>>
      such_that_clauses = QueryParser::ExtractSuchThatClauses(such_that_tokens, declarations);

  REQUIRE(such_that_clauses.size() == 1);

  std::unique_ptr<SuchThatClause> such_that_clause = std::move(such_that_clauses[0]);
  auto *clause = dynamic_cast<UsesS *>(such_that_clause.get());
  RefParam expectedRhs = EntRef(declarations[0]);
  RefParam expectedLhs = StmtRef(14);
  REQUIRE(SuchThatClause::are_stmt_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE("Query Parser can extract such that clauses with UsesP('entRef', 'entRef') relation") {
  std::string sample_query = "variable v;\n"
                             "Select v such that Uses (\"main\", v)";
  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(sample_query);
  std::vector<Declaration> declarations = QueryParser::ExtractDeclarations(tokens);
  std::vector<QueryToken> such_that_tokens = QueryParser::ExtractSuchThatTokens(tokens);
  std::vector<std::unique_ptr<SuchThatClause>>
      such_that_clauses = QueryParser::ExtractSuchThatClauses(such_that_tokens, declarations);

  REQUIRE(such_that_clauses.size() == 1);

  std::unique_ptr<SuchThatClause> such_that_clause = std::move(such_that_clauses[0]);
  auto *clause = dynamic_cast<UsesP *>(such_that_clause.get());
  RefParam expectedRhs = EntRef(declarations[0]);
  RefParam expectedLhs = EntRef("main");
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->lhs, expectedLhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->rhs, expectedRhs));
}

TEST_CASE(
    "Query Parser can extract multiple such that clauses with UsesS('stmtRef', 'stmtRef') UsesP('entRef', 'entRef') relation") {
  std::string sample_query = "variable v;\n"
                             "Select v such that Uses(14, v) Uses (\"main\", v)";
  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(sample_query);
  std::vector<Declaration> declarations = QueryParser::ExtractDeclarations(tokens);
  std::vector<QueryToken> such_that_tokens = QueryParser::ExtractSuchThatTokens(tokens);
  std::vector<std::unique_ptr<SuchThatClause>>
      such_that_clauses = QueryParser::ExtractSuchThatClauses(such_that_tokens, declarations);

  REQUIRE(such_that_clauses.size() == 2);

  std::unique_ptr<SuchThatClause> first_such_that_clause = std::move(such_that_clauses[0]);
  auto *first_clause = dynamic_cast<UsesS *>(first_such_that_clause.get());
  RefParam first_expectedRhs = EntRef(declarations[0]);
  RefParam first_expectedLhs = StmtRef(14);
  REQUIRE(SuchThatClause::are_stmt_ref_equal(first_clause->lhs, first_expectedLhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(first_clause->rhs, first_expectedRhs));

  std::unique_ptr<SuchThatClause> second_such_that_clause = std::move(such_that_clauses[1]);
  auto *second_clause = dynamic_cast<UsesP *>(second_such_that_clause.get());
  RefParam second_expectedRhs = EntRef(declarations[0]);
  RefParam second_expectedLhs = EntRef("main");
  REQUIRE(SuchThatClause::are_ent_ref_equal(second_clause->lhs, second_expectedLhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(second_clause->rhs, second_expectedRhs));
}

TEST_CASE("Query Parser can return a parsed query") {
  std::vector<QueryToken> tokens = {
      {"variable", PQLTokenType::DECLARATION},
      {"v1", PQLTokenType::SYNONYM},
      {"v2", PQLTokenType::SYNONYM},
      {"assign", PQLTokenType::DECLARATION},
      {"a1", PQLTokenType::SYNONYM},
      {"a2", PQLTokenType::SYNONYM},
      {"Select", PQLTokenType::SELECT},
      {"v2", PQLTokenType::SYNONYM}
  };
  ParsedQuery parsedQuery = QueryParser::ParseTokenizedQuery(tokens);
  SelectClause expected;
  expected.synonym = "v2";
  expected.design_entity = DesignEntity::VARIABLE;
  REQUIRE(parsedQuery.select.equals(expected));
}
