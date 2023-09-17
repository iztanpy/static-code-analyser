#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include "qps/qps_errors/qps_error.h"

class QpsSyntaxError : public Error {
 public:
  explicit QpsSyntaxError(const std::string & message) : Error(message) {}

  const char *what() const noexcept override;
};
