#pragma once

#include <unordered_set>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

struct SuchThatValidator {
  /*!
   * Rules for validating Pattern clauses
   */
  inline static const std::unordered_set<DesignEntity> kUsesModifiesValidRhs = {DesignEntity::VARIABLE};

  inline static const std::unordered_set<DesignEntity> kUsesValidLhs =
      {DesignEntity::STMT, DesignEntity::ASSIGN, DesignEntity::PRINT, DesignEntity::IF_STMT,
       DesignEntity::WHILE_LOOP, DesignEntity::CALL, DesignEntity::PROCEDURE};

  inline static const std::unordered_set<DesignEntity> kModifiesValidLhs =
      {DesignEntity::STMT, DesignEntity::ASSIGN, DesignEntity::READ, DesignEntity::IF_STMT,
       DesignEntity::WHILE_LOOP, DesignEntity::CALL, DesignEntity::PROCEDURE};

  inline static const std::unordered_set<DesignEntity> kParentFollowsValidRhsLhs =
      {DesignEntity::STMT, DesignEntity::ASSIGN, DesignEntity::READ, DesignEntity::PRINT, DesignEntity::IF_STMT,
       DesignEntity::WHILE_LOOP, DesignEntity::CALL};

  /*!
   * Validates the Parent and Follow clause
   * @param lhs is the right hand side of the Parent/Follow clause
   * @param rhs is the left hand side of the Parent/Follow clause
   * Will throw QpsSemanticError if the Parent/Follow clause is invalid
   */
  static void ValidateParentFollows(StmtRef lhs, StmtRef rhs);

  /*!
   * Validates the UsesS/UsesP clause
   * @param lhs is the right hand side of the UsesS/UsesP clause
   * @param rhs is the left hand side of the UsesS/UsesP clause
   * Will throw QpsSemanticError if the UsesS/UsesP clause is invalid
   */
  static void ValidateUses(StmtRef lhs, EntRef rhs);
  static void ValidateUses(EntRef lhs, EntRef rhs);

  /*!
   * Validates the ModifiesS/ModifiesP clause
   * @param lhs is the right hand side of the ModifiesS/ModifiesP clause
   * @param rhs is the left hand side of the ModifiesS/ModifiesP clause
   * Will throw QpsSemanticError if the ModifiesS/ModifiesP clause is invalid
   */
  static void ValidateModifies(StmtRef lhs, EntRef rhs);
  static void ValidateModifies(EntRef lhs, EntRef rhs);
};
