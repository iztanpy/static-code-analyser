#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include "qps/qps_errors/qps_error.h"

class QpsSemanticError : public QpsError {
 public:
  explicit QpsSemanticError(const std::string& message) : QpsError(message) {}
};
