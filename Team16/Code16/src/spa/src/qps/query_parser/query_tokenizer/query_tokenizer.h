#pragma once

#include <string>
#include <vector>

// Token types
enum class PQLTokenType {
  SYNONYM,
  DECLARATION,
  SELECT,
  SUCH_THAT,
  PATTERN,
  WILDCARD,
  RELREF,
  INTEGER,
  IDENT,
  PARTIALEXPR
};

struct QueryToken {
  std::string text;
  PQLTokenType type;
};

class QueryTokenizer {
 public:
  static std::vector<QueryToken> tokenize(const std::string & query);
  static bool isEnclosedInDoubleQuotes(const std::string & token);
  static bool isEnclosedInWildcards(const std::string & token);
};
