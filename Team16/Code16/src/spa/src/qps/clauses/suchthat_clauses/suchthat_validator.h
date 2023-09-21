#pragma once

#include <unordered_set>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

struct SuchThatValidator {
  inline static const std::unordered_set<DesignEntity> kUsesValidRhs = {DesignEntity::VARIABLE};
  inline static const std::unordered_set<DesignEntity> kUsesValidLhs =
      {DesignEntity::STMT, DesignEntity::ASSIGN, DesignEntity::PRINT, DesignEntity::IF_STMT,
       DesignEntity::WHILE_LOOP, DesignEntity::CALL, DesignEntity::PROCEDURE};

  inline static const std::unordered_set<DesignEntity> kParentFollowsValidRhsLhs =
      {DesignEntity::STMT, DesignEntity::ASSIGN, DesignEntity::READ, DesignEntity::PRINT, DesignEntity::IF_STMT,
       DesignEntity::WHILE_LOOP, DesignEntity::CALL};

  static void ValidateParentFollow(StmtRef lhs, StmtRef rhs);
  static void ValidateUse(StmtRef lhs, EntRef rhs);
  static void ValidateUse(EntRef lhs, EntRef rhs);
};
