#pragma once

#include <unordered_set>
#include "qps/design_entity.h"
#include "utils/clauses_types.h"
#include "qps/qps_errors/qps_semantic_error.h"

/*!
 * Validates the Pattern clauses
 */
struct PatternValidator {
  /*!
   * Rules for validating Pattern clauses
   */
  inline static const DesignEntity kValidAssign = DesignEntity::ASSIGN;
  inline static const DesignEntity kValidWhile = DesignEntity::WHILE_LOOP;
  inline static const DesignEntity kValidIf = DesignEntity::IF_STMT;
  inline static const DesignEntity kValidLhs = DesignEntity::VARIABLE;

  /*!
   * Validates the Pattern clause
   * @param leading is the leading synonym before "(" in pattern
   * @param lhs is the left hand side of the Pattern clause
   * Will throw QpsSemanticError if the Pattern clause is invalid
   */
  static void ValidateAssign(Declaration& leading, EntRef& lhs);
  static void ValidateWhile(Declaration& leading, EntRef& lhs);
  static void ValidateIf(Declaration& leading, EntRef& lhs);

  static void ValidateLhs(EntRef& lhs);
};
