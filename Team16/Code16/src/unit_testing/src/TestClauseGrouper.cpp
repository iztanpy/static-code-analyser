#include "catch.hpp"

#include "qps/query_evaluator/constraint_solver/clause_grouper.h"
#include "qps/query_evaluator/constraint_solver/clause_group.h"
#include "qps/clauses/with_clauses/with_clause.h"
#include "qps/clauses/pattern_clauses/assign.h"

TEST_CASE("ClauseGrouper::GetClauseGroupOrder", "[ClauseGrouper]") {
  ClauseGrouper clauseGrouper = ClauseGrouper();

  AttrRef attr_ref_1 = {{"v", DesignEntity::VARIABLE}, AttrName::VARNAME};
  AttrRef attr_ref_2 = {{"p", DesignEntity::PRINT}, AttrName::VARNAME};
  std::unique_ptr<Clause> clause1 = std::make_unique<WithClause>(attr_ref_1, attr_ref_2, true);

  SECTION("Without normal clause, should top up 2 selects") {
    clauseGrouper.addClause(std::move(clause1));
    std::vector<ClauseGroup> clauseGroups = clauseGrouper.GetClauseGroupOrder();
    REQUIRE(clauseGroups.size() == 1);
    REQUIRE(clauseGroups[0].SizeForTesting() == 3);
  }

  SECTION("With overlapping normal clause, should top up 1 selects") {
    std::unique_ptr<Clause> clause2 = std::make_unique<AssignPattern>(
        Declaration{"a", DesignEntity::ASSIGN},
        Declaration{"v", DesignEntity::VARIABLE},
        PartialExpr{"x"},
        false
    );

    clauseGrouper.addClause(std::move(clause1));
    clauseGrouper.addClause(std::move(clause2));

    std::vector<ClauseGroup> clauseGroups = clauseGrouper.GetClauseGroupOrder();
    REQUIRE(clauseGroups.size() == 1);
    REQUIRE(clauseGroups[0].SizeForTesting() == 3);
  }
}