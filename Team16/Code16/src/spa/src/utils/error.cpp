#include "utils/error.h"

void InvalidStatementError::log() const {
  std::cerr << "error: " << message << std::endl;
  exit(1);
}

void InvalidTokenTypeError::log() const {
  std::cerr << "error: " << message << std::endl;
  exit(1);
}

void InvalidSyntaxError::log() const {
  std::cerr << "error: " << message << std::endl;
  exit(1);
}

void InvalidSemanticError::log() const {
  std::cerr << "error: " << message << std::endl;
  exit(1);
}
