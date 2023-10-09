#include <catch.hpp>
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/query_parser/query_parser.h"
#include "qps/clauses/suchthat_clauses/suchthat_clauses_all.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "qps/clauses/suchthat_clauses/calls.h"

TEST_CASE("Query Parser can extract select clause") {
  std::string sample_query = "variable v; Select v";
  TokenisedQuery tokenised_query = QueryTokenizer::tokenize(sample_query);
  std::vector<QueryToken> selectTokens = tokenised_query.select_tokens;
  std::vector<Declaration> declarations = tokenised_query.declarations;
  std::vector<SelectClause> selectClause = QueryParser::ExtractSelectClauses(selectTokens, declarations);
  REQUIRE(selectClause.size() == 1);
  REQUIRE(selectClause[0].declaration.design_entity == DesignEntity::VARIABLE);
  REQUIRE(selectClause[0].declaration.synonym == "v");
}

TEST_CASE(
    "Query Parser can extract multiple such that clauses with UsesP('entRef', 'entRef') relation") {
  std::string sample_query = "variable v;\n Select v such that Uses(\"main\", v)";
  TokenisedQuery tokenised_query = QueryTokenizer::tokenize(sample_query);
  std::vector<Declaration> declarations = tokenised_query.declarations;
  std::vector<QueryToken> such_that_tokens = tokenised_query.such_that_tokens;
  std::vector<std::unique_ptr<SuchThatClause>>
      such_that_clauses = QueryParser::ExtractSuchThatClauses(such_that_tokens, declarations);

  REQUIRE(such_that_clauses.size() == 1);

  std::unique_ptr<SuchThatClause> such_that_clause = std::move(such_that_clauses[0]);
  auto* clause = dynamic_cast<UsesP*>(such_that_clause.get());
  RefParam expected_rhs = EntRef(declarations[0]);
  RefParam expected_lhs = EntRef("main");
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->lhs, expected_lhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->rhs, expected_rhs));
}

TEST_CASE("Query parser can extract pattern clause 'a (entRef, expr)'") {
  std::string sample_query = "variable v; assign a; Select v pattern a(v, _)";
  TokenisedQuery tokenised_query = QueryTokenizer::tokenize(sample_query);
  std::vector<Declaration> declarations = tokenised_query.declarations;
  std::vector<QueryToken> pattern_tokens = tokenised_query.pattern_tokens;
  std::vector<std::unique_ptr<PatternClause>>
      pattern_clauses = QueryParser::ExtractPatternClauses(pattern_tokens, declarations);

  REQUIRE(pattern_clauses.size() == 1);

  std::unique_ptr<PatternClause> pattern_clause = std::move(pattern_clauses[0]);
  auto* clause = dynamic_cast<AssignPattern*>(pattern_clause.get());

  EntRef expected_lhs = EntRef(declarations[0]);
  ExprSpec expected_rhs = Wildcard::Value;
  REQUIRE(clause->syn_assignment.equals(declarations[1]));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->lhs, expected_lhs));
  REQUIRE(PatternClause::are_expr_spec_equal(clause->rhs, expected_rhs));
}

TEST_CASE("Query Parser can return a parsed query") {
  std::string sample_pattern_query = "variable v; assign a; Select v pattern a(v, _)";
  ParsedQuery parsed_pattern_query = QueryParser::ParseTokenizedQuery(sample_pattern_query);
  std::vector<Declaration> declarations = {
      {"v", DesignEntity::VARIABLE},
      {"a", DesignEntity::ASSIGN}
  };

  SelectClause expected;
  expected.declaration = {"v", DesignEntity::VARIABLE};

  EntRef expected_lhs = EntRef(declarations[0]);
  EntRef expected_rhs = EntRef(Wildcard::Value);

  std::unique_ptr<PatternClause> pattern_clause = std::move(parsed_pattern_query.pattern_clauses[0]);
  std::vector<std::unique_ptr<SuchThatClause>> such_that_clauses = std::move(parsed_pattern_query.such_that_clauses);

  REQUIRE(parsed_pattern_query.select.equals(expected));
  REQUIRE(parsed_pattern_query.such_that_clauses.empty());

  auto* clause = dynamic_cast<AssignPattern*>(pattern_clause.get());
  REQUIRE(clause->syn_assignment.equals(declarations[1]));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->lhs, expected_lhs));
  REQUIRE(PatternClause::are_expr_spec_equal(clause->rhs, Wildcard::Value));
}

TEST_CASE("Parser can parse Calls and Calls*") {
  std::string sample_query_1 = "procedure p; Select p such that Calls(p, _)";
  ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query_1);
  std::vector<Declaration> declarations = {
      {"p", DesignEntity::PROCEDURE}
  };

  std::vector<std::unique_ptr<SuchThatClause>> such_that_clauses = std::move(parsed_query.such_that_clauses);

  EntRef expected_lhs = EntRef(declarations[0]);
  EntRef expected_rhs = EntRef(Wildcard::Value);

  REQUIRE(such_that_clauses.size() == 1);

  std::unique_ptr<SuchThatClause> such_that_clause = std::move(such_that_clauses[0]);
  auto* clause = dynamic_cast<Calls*>(such_that_clause.get());
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->lhs, expected_lhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause->rhs, expected_rhs));

  std::string sample_query_2 = "procedure p; Select p such that Calls*(p, \"Third\")";
  ParsedQuery parsed_query_2 = QueryParser::ParseTokenizedQuery(sample_query_2);
  std::vector<Declaration> declarations_2 = {
      {"p", DesignEntity::PROCEDURE}
  };

  std::vector<std::unique_ptr<SuchThatClause>> such_that_clauses_2 = std::move(parsed_query_2.such_that_clauses);

  EntRef expected_lhs_2 = EntRef(declarations_2[0]);
  EntRef expected_rhs_2 = EntRef("Third");

  REQUIRE(such_that_clauses.size() == 1);

  std::unique_ptr<SuchThatClause> such_that_clause_2 = std::move(such_that_clauses_2[0]);
  auto* clause_2 = dynamic_cast<CallsT*>(such_that_clause_2.get());
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause_2->lhs, expected_lhs_2));
  REQUIRE(SuchThatClause::are_ent_ref_equal(clause_2->rhs, expected_rhs_2));
}

TEST_CASE("Query parser throws correct errors") {
  std::string sample_query = "Select s";
  REQUIRE_THROWS_AS(QueryParser::ParseTokenizedQuery(sample_query), QpsSemanticError);

  std::string sample_query_2 = "Select 1v";
  REQUIRE_THROWS_AS(QueryParser::ParseTokenizedQuery(sample_query_2), QpsSyntaxError);
}
