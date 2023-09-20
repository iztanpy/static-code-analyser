// ai-gen start (gpt3, 2)
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/design_entity.h"
#include "qps/rel_ref.h"

std::vector<QueryToken> QueryTokenizer::tokenize(const std::string & query) {
  std::vector<QueryToken> tokens;
  std::string sanitizedQuery;

  // Replace multiple consecutive whitespaces and new lines with a single space
  bool inWhitespace = false;
  std::set<char> queryDelimiters{',', ';', '(', ')'};

  for (char c : query) {
    if (std::isspace(c) || queryDelimiters.find(c) != queryDelimiters.end()) {
      if (!inWhitespace) {
        sanitizedQuery += ' ';
        inWhitespace = true;
      }
    } else {
      sanitizedQuery += c;
      inWhitespace = false;
    }
  }

  std::istringstream tokenizer(sanitizedQuery);
  std::string token;
  std::set < std::string > stringDesignEntities = Entity::getStringDesignEntities();
  std::set < std::string > stringRelRef = RelRef::getStringRelRef();

  while (std::getline(tokenizer, token, ' ')) {
    if (!token.empty()) {
      // Determine the token type
      PQLTokenType type;

      // First check if the token is numeric (an integer)
      bool isNumeric = true;
      for (char c : token) {
        if (!std::isdigit(c)) {
          isNumeric = false;
          break;
        }
      }

      // Check if the token contain "_"expr"_" => must be a partial expression
      bool isPartialExpr = isEnclosedInWildcards(token);
      // Check if the token contains ""
      bool containsDoubleQuotes = isEnclosedInDoubleQuotes(token);

      if (isNumeric) {
        type = PQLTokenType::INTEGER;
      } else if (isPartialExpr) {
        type = PQLTokenType::PARTIALEXPR;
        token.erase(0, 1);  // Remove the first _
        token.erase(token.size() - 1);  // Remove the last _
        token.erase(0, 1);  // Remove the first double quote
        token.erase(token.size() - 1);  // Remove the last double quote
      } else if (containsDoubleQuotes) {
        type = PQLTokenType::IDENT;
        token.erase(0, 1);  // Remove the first double quote
        token.erase(token.size() - 1);  // Remove the last double quote
      } else if (stringDesignEntities.find(token) != stringDesignEntities.end()) {
        type = PQLTokenType::DECLARATION;
      } else if (stringRelRef.find(token) != stringRelRef.end()) {
        // TODO(sumian): ensure that in the validator, a relationship ref can only exist in a such that clause
        type = PQLTokenType::RELREF;
      } else if (token == "_") {
        type = PQLTokenType::WILDCARD;
      } else if (token == "Select") {
        type = PQLTokenType::SELECT;
      } else if (token == "that" || token == "such") {
        // TODO(sumian): ensure that in the validator, 'that' follows a 'such'
        type = PQLTokenType::SUCH_THAT;
      } else if (token == "pattern") {
        type = PQLTokenType::PATTERN;
      } else {
        type = PQLTokenType::SYNONYM;
      }

      // Store the token and its type
      tokens.push_back({token, type});
    }
  }

  return
      tokens;
}

bool QueryTokenizer::isEnclosedInDoubleQuotes(const std::string & token) {
  return (token.size() >= 2 && token.front() == '\"' && token.back() == '\"');
}

bool QueryTokenizer::isEnclosedInWildcards(const std::string & token) {
  return (token.size() >= 2 && token.front() == '_' && token.back() == '_');
}
// ai-gen end
