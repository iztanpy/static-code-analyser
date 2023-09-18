#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include "qps/qps_errors/qps_error.h"

class QpsSyntaxError : public QpsError {
 public:
  explicit QpsSyntaxError(const std::string& message) : QpsError(message) {}

  const char* what() const noexcept override;
};
