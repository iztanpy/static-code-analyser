#include <catch.hpp>
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/query_parser/query_parser.h"
#include "qps/clauses/suchthat_clauses/suchthat_clauses_all.h"
#include "qps/qps_errors/qps_syntax_error.h"

TEST_CASE("Query Parser can extract select clause") {
  std::string sample_query = "variable v; Select v";
  TokenisedQuery tokenised_query = QueryTokenizer::tokenize(sample_query);
  std::vector<QueryToken> selectTokens = tokenised_query.select_tokens;
  std::vector<Declaration> declarations = tokenised_query.declarations;
  std::vector<std::unique_ptr<Clause>> selectClause = QueryParser::ExtractSelectClauses(selectTokens, declarations);
  REQUIRE(selectClause.size() == 1);
  auto* clause = dynamic_cast<SelectClause*>(selectClause[0].get());
  REQUIRE(clause->declaration.design_entity == DesignEntity::VARIABLE);
  REQUIRE(clause->declaration.synonym == "v");
}

TEST_CASE(
    "Query Parser can extract multiple such that clauses with UsesP('entRef', 'entRef') relation") {
  std::string sample_query = "variable v;\n Select v such that Uses(\"main\", v)";
  TokenisedQuery tokenised_query = QueryTokenizer::tokenize(sample_query);
  std::vector<Declaration> declarations = tokenised_query.declarations;
  std::vector<QueryToken> such_that_tokens = tokenised_query.such_that_tokens;
  std::vector<std::unique_ptr<Clause>>
      such_that_clauses = QueryParser::ExtractSuchThatClauses(such_that_tokens, declarations);

  REQUIRE(such_that_clauses.size() == 1);

  std::unique_ptr<Clause> such_that_clause = std::move(such_that_clauses[0]);
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
  std::vector<std::unique_ptr<Clause>>
      pattern_clauses = QueryParser::ExtractPatternClauses(pattern_tokens, declarations);

  REQUIRE(pattern_clauses.size() == 1);

  std::unique_ptr<Clause> pattern_clause = std::move(pattern_clauses[0]);
  auto* clause = dynamic_cast<AssignPattern*>(pattern_clause.get());

  EntRef expected_lhs = EntRef(declarations[0]);
  ExprSpec expected_rhs = Wildcard::Value;
  REQUIRE(clause->declaration.equals(declarations[1]));
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

  SelectClause expected_select_clause;
  expected_select_clause.declaration = declarations[0];

  std::vector<Synonym> expected_selects = {declarations[0].synonym};

  EntRef expected_lhs = EntRef(declarations[0]);
  EntRef expected_rhs = EntRef(Wildcard::Value);

  auto* select_clause = dynamic_cast<SelectClause*>(std::move(parsed_pattern_query.clauses[0]).get());
  auto* assign_clause = dynamic_cast<AssignPattern*>(std::move(parsed_pattern_query.clauses[1]).get());

  REQUIRE(parsed_pattern_query.clauses.size() == 2);
  REQUIRE(parsed_pattern_query.selects == expected_selects);
  REQUIRE(select_clause->equals(expected_select_clause));
  REQUIRE(assign_clause->declaration.equals(declarations[1]));
  REQUIRE(SuchThatClause::are_ent_ref_equal(assign_clause->lhs, expected_lhs));
  REQUIRE(PatternClause::are_expr_spec_equal(assign_clause->rhs, Wildcard::Value));
}

TEST_CASE("Parser can parse Calls and Calls*") {
  std::string sample_query_1 = "procedure p; Select p such that Calls(p, _)";
  ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query_1);
  std::vector<Declaration> declarations = {
      {"p", DesignEntity::PROCEDURE}
  };

  SelectClause expected_select_clause;
  expected_select_clause.declaration = declarations[0];

  std::vector<Synonym> expected_selects = {declarations[0].synonym};

  EntRef expected_lhs = EntRef(declarations[0]);
  EntRef expected_rhs = EntRef(Wildcard::Value);

  auto* select_clause = dynamic_cast<SelectClause*>(std::move(parsed_query.clauses[0]).get());
  auto* calls_clause = dynamic_cast<Calls*>(std::move(parsed_query.clauses[1]).get());

  REQUIRE(parsed_query.clauses.size() == 2);
  REQUIRE(parsed_query.selects == expected_selects);
  REQUIRE(select_clause->equals(expected_select_clause));
  REQUIRE(SuchThatClause::are_ent_ref_equal(calls_clause->lhs, expected_lhs));
  REQUIRE(SuchThatClause::are_ent_ref_equal(calls_clause->rhs, expected_rhs));

  std::string sample_query_2 = "procedure p; Select p such that Calls*(p, \"Third\")";
  ParsedQuery parsed_query_2 = QueryParser::ParseTokenizedQuery(sample_query_2);
  std::vector<Declaration> declarations_2 = {
      {"p", DesignEntity::PROCEDURE}
  };

  SelectClause expected_select_clause_2;
  expected_select_clause_2.declaration = declarations_2[0];

  std::vector<Synonym> expected_selects_2 = {declarations_2[0].synonym};

  EntRef expected_lhs_2 = EntRef(declarations_2[0]);
  EntRef expected_rhs_2 = EntRef("Third");

  auto* select_clause_2 = dynamic_cast<SelectClause*>(std::move(parsed_query_2.clauses[0]).get());
  auto* calls_clause_2 = dynamic_cast<CallsT*>(std::move(parsed_query_2.clauses[1]).get());

  REQUIRE(parsed_query_2.clauses.size() == 2);
  REQUIRE(parsed_query_2.selects == expected_selects_2);
  REQUIRE(select_clause_2->equals(expected_select_clause_2));
  REQUIRE(SuchThatClause::are_ent_ref_equal(calls_clause_2->lhs, expected_lhs_2));
  REQUIRE(SuchThatClause::are_ent_ref_equal(calls_clause_2->rhs, expected_rhs_2));
}

TEST_CASE("Query parser throws correct errors") {
  std::string sample_query = "Select s";
  REQUIRE_THROWS_AS(QueryParser::ParseTokenizedQuery(sample_query), QpsSemanticError);

  std::string sample_query_2 = "Select 1v";
  REQUIRE_THROWS_AS(QueryParser::ParseTokenizedQuery(sample_query_2), QpsSyntaxError);
}

TEST_CASE("Parser can parse Next and Next*") {
  std::string sample_query_1 = "stmt s1; Select s1 such that Next(2, 3)";
  ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query_1);
  std::vector<Declaration> declarations = {
      {"s1", DesignEntity::STMT}
  };

  SelectClause expected_select_clause;
  expected_select_clause.declaration = declarations[0];

  std::vector<Synonym> expected_selects = {declarations[0].synonym};

  StmtRef expected_lhs = StmtRef(2);
  StmtRef expected_rhs = StmtRef(3);

  auto* select_clause = dynamic_cast<SelectClause*>(std::move(parsed_query.clauses[0]).get());
  auto* next_clause = dynamic_cast<Next*>(std::move(parsed_query.clauses[1]).get());

  REQUIRE(parsed_query.clauses.size() == 2);
  REQUIRE(parsed_query.selects == expected_selects);
  REQUIRE(select_clause->equals(expected_select_clause));
  REQUIRE(SuchThatClause::are_stmt_ref_equal(next_clause->lhs, expected_lhs));
  REQUIRE(SuchThatClause::are_stmt_ref_equal(next_clause->rhs, expected_rhs));

  std::string sample_query_2 = "procedure p; Select p such that Next*(2, 9)";
  ParsedQuery parsed_query_2 = QueryParser::ParseTokenizedQuery(sample_query_2);
  std::vector<Declaration> declarations_2 = {
      {"p", DesignEntity::PROCEDURE}
  };

  SelectClause expected_select_clause_2;
  expected_select_clause_2.declaration = declarations_2[0];

  std::vector<Synonym> expected_selects_2 = {declarations_2[0].synonym};

  StmtRef expected_lhs_2 = StmtRef(2);
  StmtRef expected_rhs_2 = StmtRef(9);

  auto* select_clause_2 = dynamic_cast<SelectClause*>(std::move(parsed_query_2.clauses[0]).get());
  auto* next_clause_2 = dynamic_cast<NextT*>(std::move(parsed_query_2.clauses[1]).get());

  REQUIRE(parsed_query_2.clauses.size() == 2);
  REQUIRE(parsed_query_2.selects == expected_selects_2);
  REQUIRE(select_clause_2->equals(expected_select_clause_2));
  REQUIRE(SuchThatClause::are_stmt_ref_equal(next_clause_2->lhs, expected_lhs_2));
  REQUIRE(SuchThatClause::are_stmt_ref_equal(next_clause_2->rhs, expected_rhs_2));
}

TEST_CASE("Parser can parse Affects") {
  std::string sample_query_1 = "stmt s1; Select s1 such that Affects(2, 6)";
  ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query_1);
  std::vector<Declaration> declarations = {
      {"s1", DesignEntity::STMT}
  };

  SelectClause expected_select_clause;
  expected_select_clause.declaration = declarations[0];

  std::vector<Synonym> expected_selects = {declarations[0].synonym};

  StmtRef expected_lhs = StmtRef(2);
  StmtRef expected_rhs = StmtRef(6);

  auto* select_clause = dynamic_cast<SelectClause*>(std::move(parsed_query.clauses[0]).get());
  auto* affects_clause = dynamic_cast<Affects*>(std::move(parsed_query.clauses[1]).get());

  REQUIRE(parsed_query.clauses.size() == 2);
  REQUIRE(parsed_query.selects == expected_selects);
  REQUIRE(select_clause->equals(expected_select_clause));
  REQUIRE(SuchThatClause::are_stmt_ref_equal(affects_clause->lhs, expected_lhs));
  REQUIRE(SuchThatClause::are_stmt_ref_equal(affects_clause->rhs, expected_rhs));
}

TEST_CASE("Parser can parse while pattern") {
  std::string sample_query_1 = "while w;\n"
                               "Select w pattern w (\"x\", _)";
  ParsedQuery parsed_query_1 = QueryParser::ParseTokenizedQuery(sample_query_1);
  std::vector<Declaration> declarations = {
      {"w", DesignEntity::WHILE_LOOP}
  };

  SelectClause expected_select_clause;
  expected_select_clause.declaration = declarations[0];

  std::vector<Synonym> expected_selects = {declarations[0].synonym};

  auto* select_clause = dynamic_cast<SelectClause*>(std::move(parsed_query_1.clauses[0]).get());
  auto* while_clause = dynamic_cast<WhilePattern*>(std::move(parsed_query_1.clauses[1]).get());

  REQUIRE(parsed_query_1.clauses.size() == 2);
  REQUIRE(parsed_query_1.selects == expected_selects);
  REQUIRE(select_clause->equals(expected_select_clause));
  REQUIRE(while_clause->declaration.equals(declarations[0]));
}

TEST_CASE("Parser can parse if pattern") {
  std::string sample_query_1 = "if ifs;\n"
                               "Select ifs pattern ifs (_, _, _)";
  ParsedQuery parsed_query_1 = QueryParser::ParseTokenizedQuery(sample_query_1);
  std::vector<Declaration> declarations = {
      {"ifs", DesignEntity::IF_STMT}
  };

  SelectClause expected_select_clause;
  expected_select_clause.declaration = declarations[0];

  std::vector<Synonym> expected_selects = {declarations[0].synonym};

  auto* select_clause = dynamic_cast<SelectClause*>(std::move(parsed_query_1.clauses[0]).get());
  auto* if_clause = dynamic_cast<IfPattern*>(std::move(parsed_query_1.clauses[1]).get());

  REQUIRE(parsed_query_1.clauses.size() == 2);
  REQUIRE(parsed_query_1.selects == expected_selects);
  REQUIRE(select_clause->equals(expected_select_clause));
  REQUIRE(if_clause->declaration.equals(declarations[0]));
}
