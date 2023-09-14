#pragma once

#include "qps/qps_errors/qps_semantic_error.h"

const char *QpsSemanticError::what() const noexcept {
  return "Semantic Error";
}
