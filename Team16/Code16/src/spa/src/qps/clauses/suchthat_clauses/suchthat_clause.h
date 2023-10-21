#pragma once

#include <string>
#include <variant>
#include <unordered_set>
#include <utility>
#include <cstdint>

#include "qps/declaration.h"
#include "qps/clauses/clause.h"
#include "qps/rel_ref.h"
#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "qps/qps_errors/qps_semantic_error.h"
#include "utils/clauses_types.h"
#include "PKB/API/ReadFacade.h"

/**
 * Represents a such that clause in a parsed query. Query Evaluator
 * will use this to build an Evaluator object. Evaluator will then
 * call PKB to get back valid values
 */
class SuchThatClause : public Clause {
 public:
  RefParam lhs;
  RefParam rhs;

  SuchThatClause(RefParam lhs, RefParam rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {}

  /*!
   * Checks if two RefParams are equal
   * @param param_1
   * @param param_2
   * @return true if they are equal, else false
   */
  static bool are_stmt_ref_equal(const RefParam& param_1, const RefParam& param_2);
  static bool are_ent_ref_equal(const RefParam& param_1, const RefParam& param_2);
  static bool are_stmt_ref(const RefParam& param_1, const RefParam& param_2);
  static bool are_ent_ref(const RefParam& param_1, const RefParam& param_2);
  static bool are_stmt_decl(const StmtRef& param_1, const StmtRef& param_2);
  static bool are_ent_decl(const EntRef& param_1, const EntRef& param_2);
  static bool are_stmt_wildcard(const StmtRef& param_1, const StmtRef& param_2);
  static bool are_ent_wildcard(const EntRef& param_1, const EntRef& param_2);

  /*!
   * Evaluate this clause given info from PKB
   * @param pkb_reader
   * @return Constraint that contains all possible valid values for this clause
   */
  Constraint Evaluate(ReadFacade& pkb_reader) override = 0;

  /*!
   * Gets the synonyms used in this SuchThat clause. If SuchThat clause has 2 synonyms
   * of the same name, it will only return 1 due to unordered_set
   * @return a set of elements
   */
  std::unordered_set<Synonym> GetSynonyms() const override;

  ~SuchThatClause() override = default;

  /*!
   * Functions to support hashing of clauses
   */
  size_t Hash() const override;

  RelRefType GetRelRef() const override = 0;

  bool equals(const Clause* other) const override;

  // Overloaded == operator
  friend bool operator==(const SuchThatClause& lhs, const SuchThatClause& rhs);

 private:
  /*!
   * Validates the pattern clause semantically
   * Will throw QpsSemanticError if the clause is initialized with invalid arguments
   */
  virtual void Validate() = 0;
};
