#include <catch.hpp>
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/query_parser/query_parser.h"
#include "qps/clauses/suchthat_clauses/suchthat_clauses_all.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "qps/clauses/with_clauses/with_clause.h"
#include "qps/clauses/pattern_clauses/assign.h"
#include "qps/clauses/pattern_clauses/while.h"
#include "qps/clauses/pattern_clauses/if.h"

bool areClauseSetsEqual(const ClauseSet& lhs, const ClauseSet& rhs) {
  if (lhs.size() != rhs.size()) {
    return false;
  }

  for (const auto& lClausePtr : lhs) {
    if (rhs.find(lClausePtr) == rhs.end()) {
      return false;
    }
  }

  return true;
}

using ClauseSet = std::unordered_set<std::unique_ptr<Clause>, ClauseHasher, ClauseEquality>;

TEST_CASE("Query Parser can extract select clause") {
  std::string sample_query = "variable v; Select v";
  TokenisedQuery tokenised_query = QueryTokenizer::tokenize(sample_query);
  std::vector<QueryToken> selectTokens = tokenised_query.select_tokens;
  std::vector<Declaration> declarations = tokenised_query.declarations;
  std::vector<std::unique_ptr<Clause>> selectClause = QueryParser::ExtractSelectClauses(selectTokens, declarations);
  REQUIRE(selectClause.size() == 1);
  auto* clause = dynamic_cast<SelectClause*>(selectClause[0].get());

  AttrRef expected_attr_ref = AttrRef(declarations[0], AttrName::NONE);
  REQUIRE(clause->attr_ref == expected_attr_ref);
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
  REQUIRE(clause->declaration == declarations[1]);
  REQUIRE(clause->lhs == expected_lhs);
  REQUIRE(clause->rhs == expected_rhs);
}

TEST_CASE("Query Parser can return a parsed query") {
  std::string sample_pattern_query = "variable v; assign a; Select v pattern a(v, _)";
  ParsedQuery parsed_pattern_query = QueryParser::ParseTokenizedQuery(sample_pattern_query);
  std::vector<Declaration> declarations = {
      {"v", DesignEntity::VARIABLE},
      {"a", DesignEntity::ASSIGN}
  };

  std::vector<std::string> expected_selects = {declarations[0].synonym};

  auto expected_select_clause_ptr = std::make_unique<SelectClause>(
      AttrRef(declarations[0], AttrName::NONE)
  );

  ClauseSet expected_clauses;
  expected_clauses.insert(std::move(expected_select_clause_ptr));
  expected_clauses.insert(std::make_unique<AssignPattern>(declarations[1],
                                                          EntRef(declarations[0]),
                                                          Wildcard::Value,
                                                          false));

  REQUIRE(parsed_pattern_query.selects == expected_selects);
  REQUIRE(areClauseSetsEqual(parsed_pattern_query.clauses, expected_clauses));
}

TEST_CASE("Parser can parse Calls and Calls*") {
  SECTION("Calls") {
    std::string sample_query_1 = "procedure p; Select p such that Calls(p, _)";
    ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query_1);
    std::vector<Declaration> declarations = {
        {"p", DesignEntity::PROCEDURE}
    };

    std::vector<std::string> expected_selects = {declarations[0].synonym};

    auto expected_select_clause_ptr = std::make_unique<SelectClause>(
        AttrRef(declarations[0], AttrName::NONE)
    );

    ClauseSet expected_clauses;
    expected_clauses.insert(std::move(expected_select_clause_ptr));
    expected_clauses.insert(std::make_unique<Calls>(EntRef(declarations[0]), Wildcard::Value, false));

    REQUIRE(parsed_query.selects == expected_selects);
    REQUIRE(areClauseSetsEqual(parsed_query.clauses, expected_clauses));
  }

  SECTION("Calls*") {
    std::string sample_query = "procedure p; Select p such that Calls*(p, \"Third\")";
    ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query);
    std::vector<Declaration> declarations = {
        {"p", DesignEntity::PROCEDURE}
    };

    std::vector<std::string> expected_selects = {declarations[0].synonym};

    auto expected_select_clause_ptr = std::make_unique<SelectClause>(
        AttrRef(declarations[0], AttrName::NONE)
    );

    ClauseSet expected_clauses;
    expected_clauses.insert(std::move(expected_select_clause_ptr));
    expected_clauses.insert(std::make_unique<CallsT>(EntRef(declarations[0]), "Third", false));

    REQUIRE(parsed_query.selects == expected_selects);
    REQUIRE(areClauseSetsEqual(parsed_query.clauses, expected_clauses));
  }
}

TEST_CASE("Query parser throws correct errors") {
  std::string sample_query = "Select s";
  REQUIRE_THROWS_AS(QueryParser::ParseTokenizedQuery(sample_query), QpsSemanticError);

  std::string sample_query_2 = "Select 1v";
  REQUIRE_THROWS_AS(QueryParser::ParseTokenizedQuery(sample_query_2), QpsSyntaxError);
}

TEST_CASE("Parser can parse Next and Next*") {
  SECTION("Next") {
    std::string sample_query_1 = "stmt s1; Select s1 such that Next(2, 3)";
    ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query_1);
    std::vector<Declaration> declarations = {
        {"s1", DesignEntity::STMT}
    };
    std::vector<std::string> expected_selects = {declarations[0].synonym};

    std::unique_ptr<SelectClause> expected_select_clause_ptr = std::make_unique<SelectClause>(
        AttrRef(declarations[0], AttrName::NONE)
    );

    ClauseSet expected_clauses;
    expected_clauses.insert(std::move(expected_select_clause_ptr));
    expected_clauses.insert(std::make_unique<Next>(2, 3, false));

    REQUIRE(parsed_query.selects == expected_selects);
    REQUIRE(areClauseSetsEqual(parsed_query.clauses, expected_clauses));
  }

  SECTION("Next*") {
    std::string sample_query = "procedure p; Select p such that Next*(2, 9)";
    ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query);
    std::vector<Declaration> declarations = {
        {"p", DesignEntity::PROCEDURE}
    };

    std::vector<std::string> expected_selects = {declarations[0].synonym};

    std::unique_ptr<SelectClause> expected_select_clause_ptr = std::make_unique<SelectClause>(
        AttrRef(declarations[0], AttrName::NONE)
    );

    ClauseSet expected_clauses;
    expected_clauses.insert(std::move(expected_select_clause_ptr));
    expected_clauses.insert(std::make_unique<NextT>(2, 9, false));

    REQUIRE(parsed_query.selects == expected_selects);
    REQUIRE(areClauseSetsEqual(parsed_query.clauses, expected_clauses));
  }
}

TEST_CASE("Parser can parse Affects") {
  std::string sample_query_1 = "stmt s1; Select s1 such that Affects(2, 6)";
  ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query_1);
  std::vector<Declaration> declarations = {
      {"s1", DesignEntity::STMT}
  };

  std::vector<std::string> expected_selects = {declarations[0].synonym};

  auto expected_select_clause_ptr = std::make_unique<SelectClause>(AttrRef(declarations[0], AttrName::NONE));

  ClauseSet expected_clauses;
  expected_clauses.insert(std::move(expected_select_clause_ptr));
  expected_clauses.insert(std::make_unique<Affects>(2, 6, false));

  REQUIRE(parsed_query.selects == expected_selects);
  REQUIRE(areClauseSetsEqual(parsed_query.clauses, expected_clauses));
}

TEST_CASE("Parser can parse while pattern") {
  std::string sample_query = "while w;\n"
                             "Select w pattern w (\"x\", _)";
  ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query);
  std::vector<Declaration> declarations = {
      {"w", DesignEntity::WHILE_LOOP}
  };

  std::vector<std::string> expected_selects = {declarations[0].synonym};

  std::unique_ptr<SelectClause>
      expected_select_clause_ptr = std::make_unique<SelectClause>(AttrRef(declarations[0], AttrName::NONE));

  ClauseSet expected_clauses;
  expected_clauses.insert(std::move(expected_select_clause_ptr));
  expected_clauses.insert(std::make_unique<WhilePattern>(declarations[0], "x", false));

  REQUIRE(parsed_query.selects == expected_selects);
  REQUIRE(areClauseSetsEqual(parsed_query.clauses, expected_clauses));
}

TEST_CASE("Parser can parse if pattern") {
  std::string sample_query = "if ifs;\n"
                             "Select ifs pattern ifs (_, _, _)";
  ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query);
  std::vector<Declaration> declarations = {
      {"ifs", DesignEntity::IF_STMT}
  };

  std::vector<std::string> expected_selects = {declarations[0].synonym};

  std::unique_ptr<SelectClause>
      expected_select_clause_ptr = std::make_unique<SelectClause>(AttrRef(declarations[0], AttrName::NONE));

  ClauseSet expected_clauses;
  expected_clauses.insert(std::move(expected_select_clause_ptr));
  expected_clauses.insert(std::make_unique<IfPattern>(declarations[0], Wildcard::Value, false));

  REQUIRE(parsed_query.selects == expected_selects);
  REQUIRE(areClauseSetsEqual(parsed_query.clauses, expected_clauses));
}

TEST_CASE("Parser can parse multiple such that clauses") {
  std::string sample_query = "assign a; while w;"
                             "Select a such that Modifies (a, \"x\") and Parent* (w, a) and Next* (1, a)";
  ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query);
  std::vector<Declaration> declarations = {
      {"a", DesignEntity::ASSIGN},
      {"w", DesignEntity::WHILE_LOOP}
  };
  std::vector<std::string> expected_selects = {declarations[0].synonym};

  std::unique_ptr<SelectClause>
      expected_select_clause_ptr = std::make_unique<SelectClause>(AttrRef(declarations[0], AttrName::NONE));

  ClauseSet expected_clauses;
  expected_clauses.insert(std::move(expected_select_clause_ptr));
  expected_clauses.insert(std::make_unique<ModifiesS>(declarations[0], "x", false));
  expected_clauses.insert(std::make_unique<ParentT>(declarations[1], declarations[0], false));
  expected_clauses.insert(std::make_unique<NextT>(1, declarations[0], false));

  REQUIRE(parsed_query.selects == expected_selects);
  REQUIRE(areClauseSetsEqual(parsed_query.clauses, expected_clauses));
}

TEST_CASE("Parser can parse multiple pattern clauses") {
  std::string sample_query = "while w, w1; if ifs; assign a;"
                             "Select w pattern w (\"x\", _) and a (\"y\", _\"x+y\"_) and ifs (\"z\", _, _)";
  ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query);
  std::vector<Declaration> declarations = {
      {"w", DesignEntity::WHILE_LOOP},
      {"a", DesignEntity::ASSIGN},
      {"ifs", DesignEntity::IF_STMT}
  };

  std::vector<std::string> expected_selects = {declarations[0].synonym};

  std::unique_ptr<SelectClause>
      expected_select_clause_ptr = std::make_unique<SelectClause>(AttrRef(declarations[0], AttrName::NONE));

  ClauseSet expected_clauses;
  expected_clauses.insert(std::move(expected_select_clause_ptr));
  expected_clauses.insert(std::make_unique<WhilePattern>(declarations[0], "x", false));
  expected_clauses.insert(std::make_unique<AssignPattern>(declarations[1], "y", PartialExpr{"((x)+(y))"}, false));
  expected_clauses.insert(std::make_unique<IfPattern>(declarations[2], "z", false));

  REQUIRE(parsed_query.selects == expected_selects);
  REQUIRE(areClauseSetsEqual(parsed_query.clauses, expected_clauses));
}

TEST_CASE("Parser can parse multiple different clauses") {
  std::string sample_query = "assign a; while w;"
                             "Select a such that Parent* (w, a) and Next* (1, a) pattern a (\"x\", _)";
  ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query);
  std::vector<Declaration> declarations = {
      {"w", DesignEntity::WHILE_LOOP},
      {"a", DesignEntity::ASSIGN},
  };

  std::vector<std::string> expected_selects = {declarations[1].synonym};

  std::unique_ptr<SelectClause>
      expected_select_clause_ptr = std::make_unique<SelectClause>(AttrRef(declarations[1], AttrName::NONE));

  ClauseSet expected_clauses;
  expected_clauses.insert(std::move(expected_select_clause_ptr));
  expected_clauses.insert(std::make_unique<ParentT>(declarations[0], declarations[1], false));
  expected_clauses.insert(std::make_unique<NextT>(1, declarations[1], false));
  expected_clauses.insert(std::make_unique<AssignPattern>(declarations[1], "x", Wildcard::Value, false));

  REQUIRE(parsed_query.selects == expected_selects);
  REQUIRE(areClauseSetsEqual(parsed_query.clauses, expected_clauses));
}

TEST_CASE("Parser can parse select BOOLEAN") {
  std::string sample_query_1 = "assign a; while w;"
                               "Select BOOLEAN such that Parent* (w, a)";
  ParsedQuery parsed_query_1 = QueryParser::ParseTokenizedQuery(sample_query_1);
  std::vector<Declaration> declarations = {
      {"w", DesignEntity::WHILE_LOOP},
      {"a", DesignEntity::ASSIGN},
  };

  REQUIRE(parsed_query_1.selects.empty());
}

TEST_CASE("Parser can parse multiple select clauses") {
  std::string sample_query_1 = "assign a; while w; stmt s;"
                               "Select <w, a, s> such that Parent* (w, a)";
  ParsedQuery parsed_query_1 = QueryParser::ParseTokenizedQuery(sample_query_1);
  std::vector<Declaration> declarations = {
      {"w", DesignEntity::WHILE_LOOP},
      {"a", DesignEntity::ASSIGN},
      {"s", DesignEntity::STMT}
  };

  std::vector<Synonym> select_clauses = {
      declarations[0].synonym,
      declarations[1].synonym,
      declarations[2].synonym
  };

  for (int i = 0; i < select_clauses.size(); i++) {
    REQUIRE(parsed_query_1.selects[i] == select_clauses[i]);
  }
}

TEST_CASE("Parser can parse with clause") {
  std::string sample_query_1 = "stmt s, s1;\n"
                               "Select s such that Follows* (s, s1) with s1.stmt#=10";
  ParsedQuery parsed_query_1 = QueryParser::ParseTokenizedQuery(sample_query_1);
  std::vector<Declaration> declarations = {
      {"s", DesignEntity::STMT},
      {"s1", DesignEntity::STMT}
  };
  std::vector<std::string> expected_selects = {declarations[0].synonym};

  std::unique_ptr<SelectClause>
      expected_select_clause_ptr = std::make_unique<SelectClause>(AttrRef(declarations[0], AttrName::NONE));

  ClauseSet expected_clauses;
  expected_clauses.insert(std::move(expected_select_clause_ptr));
  expected_clauses.insert(std::make_unique<FollowsT>(declarations[0], declarations[1], false));
  AttrRef expected_with_lhs = {declarations[1], AttrName::STMTNUM};
  expected_clauses.insert(std::make_unique<WithClause>(expected_with_lhs, 10, false));

  REQUIRE(parsed_query_1.selects == expected_selects);
  REQUIRE(areClauseSetsEqual(parsed_query_1.clauses, expected_clauses));
}

TEST_CASE("Parser can parse select attr ref") {
  std::string sample_query_1 = "assign a1, a2;\n"
                               "Select <a1.stmt#, a2> such that Affects (a1, a2)";
  ParsedQuery parsed_query_1 = QueryParser::ParseTokenizedQuery(sample_query_1);
  std::vector<Declaration> declarations = {
      {"a1", DesignEntity::ASSIGN},
      {"a2", DesignEntity::ASSIGN}
  };
  std::vector<std::string> expected_selects = {declarations[0].synonym, declarations[1].synonym};
  std::unique_ptr<SelectClause>
      expected_select_clause_ptr = std::make_unique<SelectClause>(AttrRef(declarations[0], AttrName::STMTNUM));

  std::unique_ptr<SelectClause>
      expected_select_clause_ptr_2 = std::make_unique<SelectClause>(AttrRef(declarations[1], AttrName::NONE));

  ClauseSet expected_clauses;
  expected_clauses.insert(std::move(expected_select_clause_ptr));
  expected_clauses.insert(std::move(expected_select_clause_ptr_2));
  expected_clauses.insert(std::make_unique<Affects>(declarations[0], declarations[1], false));
  REQUIRE(parsed_query_1.selects == expected_selects);
  REQUIRE(areClauseSetsEqual(parsed_query_1.clauses, expected_clauses));
}

TEST_CASE("Parser can parse not clauses") {
  std::string sample_query_1 = "read r; print p;\n"
                               "Select r.varName with not p.varName = r.varName and 5=5 and not \"x\"=\"x\" and not p.varName = \"number\"";
  ParsedQuery parsed_query_1 = QueryParser::ParseTokenizedQuery(sample_query_1);
  std::vector<Declaration> declarations_1 = {
      {"r", DesignEntity::READ},
      {"p", DesignEntity::PRINT}
  };
  std::vector<std::string> expected_selects = {declarations_1[0].synonym};
  std::unique_ptr<SelectClause>
      expected_select_clause_ptr = std::make_unique<SelectClause>(AttrRef(declarations_1[0], AttrName::VARNAME));
  ClauseSet expected_clauses;
  expected_clauses.insert(std::move(expected_select_clause_ptr));

  AttrRef expected_with_lhs_1 = {declarations_1[1], AttrName::VARNAME};
  AttrRef expected_with_rhs_1 = {declarations_1[0], AttrName::VARNAME};
  expected_clauses.insert(std::make_unique<WithClause>(expected_with_lhs_1, expected_with_rhs_1, true));
  expected_clauses.insert(std::make_unique<WithClause>(5, 5, false));
  expected_clauses.insert(std::make_unique<WithClause>("x", "x", true));
  AttrRef expected_with_lhs_2 = {declarations_1[1], AttrName::VARNAME};
  expected_clauses.insert(std::make_unique<WithClause>(expected_with_lhs_2, "number", true));
  REQUIRE(areClauseSetsEqual(parsed_query_1.clauses, expected_clauses));
}