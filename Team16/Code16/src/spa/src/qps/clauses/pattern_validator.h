#pragma once

#include <unordered_set>
#include "qps/design_entity.h"
#include "qps/clauses/clauses_types.h"
#include "qps/qps_errors/qps_semantic_error.h"

struct PatternValidator {
  inline static const std::unordered_set<DesignEntity> kValidLeading = {DesignEntity::ASSIGN};
  inline static const std::unordered_set<DesignEntity> kValidLhs = {DesignEntity::VARIABLE};

  static void Validate(Declaration& leading, EntRef& lhs);
};
