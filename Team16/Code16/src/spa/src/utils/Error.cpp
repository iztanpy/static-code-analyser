#include "Error.h"

void InvalidStatementError::log() {
std::cerr << "Error: " << message << std::endl;
}
