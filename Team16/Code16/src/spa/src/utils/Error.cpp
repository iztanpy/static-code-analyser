#include "Error.h"

void InvalidStatementError::log() {
  std::cerr << "Error: " << message << std::endl;
}

void InvalidTokenTypeError::log() {
  std::cerr << "Error: " << message << std::endl;
}
