#pragma once

#include <string>
#include <vector>

// Token types
enum class PQLTokenType {
  SYNONYM,
  DECLARATION,
  SELECT,
  SUCH_THAT,
  PATTERN
};

struct QueryToken {
  std::string text;
  PQLTokenType type;
};

class QueryTokenizer {
 public:
  static std::vector<QueryToken> tokenize(const std::string& query);
};
