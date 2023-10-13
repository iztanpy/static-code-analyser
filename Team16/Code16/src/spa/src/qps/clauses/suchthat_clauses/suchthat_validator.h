#pragma once

#include <unordered_set>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

struct SuchThatValidator {
  /*!
   * Rules for validating Pattern clauses
   */
  inline static const std::unordered_set<DesignEntity> kUsesModifiesValidLhs =
      {DesignEntity::STMT, DesignEntity::ASSIGN, DesignEntity::READ, DesignEntity::PRINT, DesignEntity::IF_STMT,
       DesignEntity::WHILE_LOOP, DesignEntity::CALL, DesignEntity::PROCEDURE};

  inline static const std::unordered_set<DesignEntity> kUsesModifiesValidRhs = {DesignEntity::VARIABLE};

  inline static const std::unordered_set<DesignEntity> kParentFollowsValidRhsLhs =
      {DesignEntity::STMT, DesignEntity::ASSIGN, DesignEntity::READ, DesignEntity::PRINT, DesignEntity::IF_STMT,
       DesignEntity::WHILE_LOOP, DesignEntity::CALL};

  inline static const DesignEntity kValidCalls = DesignEntity::PROCEDURE;

  /*!
   * Validates the Parent and Follow clause
   * @param lhs is the right hand side of the Parent/Follow clause
   * @param rhs is the left hand side of the Parent/Follow clause
   * Will throw QpsSemanticError if the Parent/Follow clause is invalid
   */
  static void ValidateParentFollows(StmtRef lhs, StmtRef rhs);

  /*!
   * Validates Uses and Modifies clause
   * @param lhs is the right hand side of the Uses and Modifies clause
   * @param rhs is the left hand side of the Uses and Modifies clause
   * Will throw QpsSemanticError if the Uses and Modifies clause is invalid
   */
  static void ValidateUsesModifies(StmtRef lhs, EntRef rhs);

  /*!
   * Validates Uses and Modifies clause
   * @param lhs is the right hand side of the Uses and Modifies clause
   * @param rhs is the left hand side of the Uses and Modifies clause
   * Will throw QpsSemanticError if the Uses and Modifies clause is invalid
   */
  static void ValidateUsesModifies(EntRef lhs, EntRef rhs);

  /*!
   * Validates Calls clause
   * @param lhs is the right hand side of the Calls clause
   * @param rhs is the left hand side of the Calls clause
   * Will throw QpsSemanticError if the Calls clause is invalid
   */
  static void ValidateCalls(EntRef lhs, EntRef rhs);
};
