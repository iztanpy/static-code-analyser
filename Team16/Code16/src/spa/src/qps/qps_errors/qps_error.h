#pragma once

#include <string>
#include <iostream>
#include <stdexcept>

// Base Error class
// TODO(phuccuongngo99): Consider sharing error with SP side
class QpsError : public std::runtime_error {
 public:
  explicit QpsError(const std::string& message) : std::runtime_error(message) {}
};
