#pragma once

#include <unordered_set>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

struct SuchThatValidator {
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

  static void ValidateParentFollows(StmtRef lhs, StmtRef rhs);
  static void ValidateUses(StmtRef lhs, EntRef rhs);
  static void ValidateUses(EntRef lhs, EntRef rhs);
  static void ValidateModifies(StmtRef lhs, EntRef rhs);
  static void ValidateModifies(EntRef lhs, EntRef rhs);
};
