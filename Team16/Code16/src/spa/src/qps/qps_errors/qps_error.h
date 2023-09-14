#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <stdexcept>

// Base Error class
class Error : public std::runtime_error {
 public:
  explicit Error(const std::string & message) : std::runtime_error(message) {}

  const char *what() const noexcept override = 0;
};
