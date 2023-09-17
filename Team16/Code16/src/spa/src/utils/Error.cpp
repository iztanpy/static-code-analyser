#include "Error.h"

void InvalidStatementError::log() const {
  std::cerr << "Error: " << message << std::endl;
}

void InvalidTokenTypeError::log() const {
  std::cerr << "Error: " << message << std::endl;
}

void InvalidSyntaxError::log() const {
  std::cerr << "Error: " << message << std::endl;
}
