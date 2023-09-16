#include "qps/qps_errors/qps_syntax_error.h"

const char* QpsSyntaxError::what() const noexcept {
  return "Syntax error";
}
