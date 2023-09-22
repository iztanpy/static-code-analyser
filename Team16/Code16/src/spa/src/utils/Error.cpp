#include "Error.h"

void InvalidStatementError::log() const {
  std::cerr << "Error: " << message << std::endl;
  exit(1);
}

void InvalidTokenTypeError::log() const {
  std::cerr << "Error: " << message << std::endl;
  exit(1);
}

void InvalidSyntaxError::log() const {
  std::cerr << "Error: " << message << std::endl;
  exit(1);
}
