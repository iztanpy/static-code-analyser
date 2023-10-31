#include <catch.hpp>

#include "qps/qps.h"
#include "qps/clauses/clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_clauses_all.h"
#include "qps/clauses/pattern_clauses/pattern_clause.h"

// TODO(phuccuongngo99): This file is to test putting clauses together in a ClauseSet
// and also QueryParser from a lot of clauses

TEST_CASE("Test 1") {
  std::string
      sample_query_1 = "procedure p; Select p such that Calls(p, _) such that Calls(_, p) such that Parent(_, _)";
  ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(sample_query_1);
  REQUIRE(parsed_query.clauses.size() == 4);
}

TEST_CASE("Test 2") {
  auto s1 = std::make_unique<SelectClause>(AttrRef({"p", DesignEntity::PROCEDURE}, AttrName::NONE));

  auto s2 = std::make_unique<SelectClause>(AttrRef({"v", DesignEntity::VARIABLE}, AttrName::NONE));

  auto s3 = std::make_unique<SelectClause>(AttrRef({"c", DesignEntity::CALL}, AttrName::PROCNAME));

  auto s4 = std::make_unique<SelectClause>(AttrRef({"c", DesignEntity::CALL}, AttrName::NONE));

  auto s5 = std::make_unique<SelectClause>(AttrRef({"c", DesignEntity::CALL}, AttrName::STMTNUM));

  auto s6 = std::make_unique<SelectClause>(AttrRef({"c", DesignEntity::CALL}, AttrName::STMTNUM));

  ClauseSet clause_set;
  clause_set.insert(std::move(s1));
  clause_set.insert(std::move(s2));
  clause_set.insert(std::move(s3));
  clause_set.insert(std::move(s4));
  clause_set.insert(std::move(s5));
  clause_set.insert(std::move(s6));

  REQUIRE(clause_set.size() == 5);
}

TEST_CASE("Test 3") {
  auto s1 = std::make_unique<Calls>(Declaration{"p", DesignEntity::PROCEDURE}, Wildcard::Value);
  auto s2 = std::make_unique<Calls>(Declaration{"x", DesignEntity::PROCEDURE}, Wildcard::Value);
//  auto s2 = std::make_unique<Parent>(Wildcard::Value, Declaration{"p", DesignEntity::STMT});

  size_t hash1 = s1->Hash();
  size_t hash2 = s2->Hash();

  ClauseSet clause_set;
  clause_set.insert(std::move(s1));
  clause_set.insert(std::move(s2));

  REQUIRE(clause_set.size() == 2);
}

TEST_CASE("Test 4") {
  auto s1 = std::make_unique<WhilePattern>(Declaration{"p", DesignEntity::WHILE_LOOP}, Wildcard::Value);
  auto s2 = std::make_unique<WhilePattern>(Declaration{"p", DesignEntity::WHILE_LOOP}, Wildcard::Value);

  ClauseSet clause_set;
  clause_set.insert(std::move(s1));
  clause_set.insert(std::move(s2));

  REQUIRE(clause_set.size() == 1);
}

TEST_CASE("Test 5") {
  auto s1 = std::make_unique<AssignPattern>(
      Declaration{"x", DesignEntity::ASSIGN}, Wildcard::Value, ExactExpr{"x"});

  auto s2 = std::make_unique<AssignPattern>(
      Declaration{"x", DesignEntity::ASSIGN}, Wildcard::Value, ExactExpr{"y"});

  ClauseSet clause_set;
  clause_set.insert(std::move(s1));
  clause_set.insert(std::move(s2));

  REQUIRE(clause_set.size() == 2);
}