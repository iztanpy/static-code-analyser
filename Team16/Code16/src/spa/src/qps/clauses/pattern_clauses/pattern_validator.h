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
  inline static const std::unordered_set<DesignEntity> kValidLeading = {DesignEntity::ASSIGN};
  inline static const std::unordered_set<DesignEntity> kValidLhs = {DesignEntity::VARIABLE};

  /*!
   * Validates the Pattern clause
   * @param leading is the leading synonym before "(" in pattern
   * @param lhs is the left hand side of the Pattern clause
   * Will throw QpsSemanticError if the Pattern clause is invalid
   */
  static void Validate(Declaration& leading, EntRef& lhs);
};
