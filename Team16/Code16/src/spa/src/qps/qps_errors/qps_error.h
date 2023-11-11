#pragma once

#include <string>
#include <iostream>
#include <stdexcept>

// Base error class
class QpsError : public std::runtime_error {
 public:
  explicit QpsError(const std::string& message) : std::runtime_error(message) {}
};
