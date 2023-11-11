/* Created by Shaine, */
// ai-gen start (gpt3, 2)
#include "SPTokeniser.h"

std::vector<std::pair<TokenType, std::regex>> regex_rules = {
    // operators
    {TokenType::kOperatorPlus, std::regex("^(\\+)")},
    {TokenType::kOperatorMinus, std::regex("^(-)")},
    {TokenType::kOperatorMultiply, std::regex("^(\\*)")},
    {TokenType::kOperatorDivide, std::regex("^(/)")},
    {TokenType::kOperatorEqual, std::regex("^(==)")},
    {TokenType::kOperatorNotEqual, std::regex("^(!=)")},
    {TokenType::kOperatorLessEqual, std::regex("^(<=)")},
    {TokenType::kOperatorLess, std::regex("^(<)")},
    {TokenType::kOperatorGreaterEqual, std::regex("^(>=)")},
    {TokenType::kOperatorGreater, std::regex("^(>)")},
    {TokenType::kOperatorLogicalAnd, std::regex("^(&&)")},
    {TokenType::kOperatorLogicalOr, std::regex("^(\\|\\|)")},
    {TokenType::kOperatorLogicalNot, std::regex("^(!)")},
    {TokenType::kOperatorMod, std::regex("^(%)")},


    // entity
    {TokenType::kEntityAssign, std::regex("^(=)")},

    // separators
    {TokenType::kSepComma, std::regex("^(,)")},
    {TokenType::kSepSemicolon, std::regex("^(;)")},
    {TokenType::kSepOpenParen, std::regex("^(\\()")},
    {TokenType::kSepCloseParen, std::regex("^(\\))")},
    {TokenType::kSepOpenBrace, std::regex("^(\\{)")},
    {TokenType::kSepCloseBrace, std::regex("^(\\})")},

    // Others
    {TokenType::kWhiteSpace, std::regex("^(\\s+)")},

    // literals
    {TokenType::kLiteralInteger, std::regex("^(0|[1-9][0-9]*)$")},
    {TokenType::kLiteralName, std::regex("^(\\b[0-9a-zA-Z]+\\b)")},
};

/**
 * @brief Splits the input source program into separate lines and extracts tokens.
 *
 * This function takes an input string representing a source program and splits it into lines.
 * It then extracts tokens based on delimiters (';', '{', '}', '(', ')', ' ', '+', '-', '=', '*', '/', '%', '<', '>',
 * '&', '|', '!', '==', '!=', '<=', '>=', '||', '&&') and adds them to the result vector.
 *
 * @param input The input source program as a single string.
 * @return A vector of strings containing the extracted tokens from the source program.
 */
std::vector<std::string> SPtokeniser::splitLines(const std::string& input) {
  std::vector<std::string> result;
  std::istringstream iss(input);
  std::unordered_set<char> delimiters = {';', '{', '}', '+', '-', '=', '(', ')',
                                         '*', '/', '%', '<', '>', '&', '|', '!', ' ', '\t', '\r', '\v', '\f', '\0'};
  std::string line;
  while (std::getline(iss, line)) {
    extractTokens(line, delimiters, result);
  }
  return result;
}

void SPtokeniser::extractTokens(const std::string& line,
                                const std::unordered_set<char>& delimiters,
                                std::vector<std::string>& result) {
  std::string currentToken;
  size_t start = 0;

  // filter out whitespaces
  size_t i = 0;
  while (i < line.length()) {
    // remove all white spaces
    std::string curr_word;
    // Iterate through each character in the line, filter out whitespaces
    size_t word_char_index = i;
    char curr_char = line[word_char_index];
    while (delimiters.find(curr_char) == delimiters.end()) {
      curr_word.push_back(curr_char);  // change to line.substr(start, i - start)
      curr_char = line[++word_char_index];
    }
    // add current word if not empty, accounts for double whitespaces and onwards
    if (!curr_word.empty()) result.push_back(curr_word);
    // include delimiters (excluding whitespaces) as tokens
    handleOperator(line, word_char_index, curr_word, delimiters, result);
    // only add non-whitespaces tokens, accounts for double whitespaces and onwards
    i = word_char_index + 1;
  }
}

void SPtokeniser::handleOperator(const std::string& line,
                                 size_t& word_char_index,
                                 std::string& curr_word,
                                 const std::unordered_set<char>& delimiters,
                                 std::vector<std::string>& result) {
  char curr_char = line[word_char_index];
  auto delimiterItr = delimiters.find(curr_char);
  if (delimiterItr != delimiters.end() && !isspace(*delimiterItr) && *delimiterItr != '\0') {
    curr_word = curr_char;
    // check if next char is a two-char operator
    if (word_char_index + 1 < line.length()) {
      char next_char = line[word_char_index + 1];
      if ((curr_word == "=" && next_char == '=')
          || (curr_word == "!" && next_char == '=')
          || (curr_word == "<" && next_char == '=')
          || (curr_word == ">" && next_char == '=')
          || (curr_word == "&" && next_char == '&')
          || (curr_word == "|" && next_char == '|')) {
        curr_word.push_back(next_char);
        word_char_index++;
      }
    }
    result.push_back(curr_word);
  }
}

/**
 * @brief Tokenizes the input source program into a vector of Token structures.
 *
 * This function takes an input source program as a string and tokenizes it, extracting tokens based on regular expressions.
 * It processes each line, identifies token types, and creates Token structures for each token found.
 * The resulting tokens are stored in a vector and returned.
 *
 * @param input The input source program as a single string.
 * @return A vector of Token structures representing the extracted tokens.
 * @throws std::runtime_error if an invalid token type or name is encountered.
 */
std::vector<struct Token> SPtokeniser::tokenise(const std::string& input) {
  std::vector<Token> tokens;
  std::vector<std::string> lines = splitLines(input);
  std::stack<char> braceStack;

  for (const std::string& line : lines) {
    TokenType matchedType = TokenType::kUnknownTokenType;
    std::string matchedValue;
    std::pair<TokenType, std::string> result = matchRegex(line);
    matchedType = result.first;
    matchedValue = result.second;

    if (matchedType == TokenType::kUnknownTokenType) { throw InvalidTokenTypeError(); }

    if (matchedType == TokenType::kSepOpenBrace || matchedType == TokenType::kSepOpenParen) {
      braceStack.push(matchedValue[0]);
    }

    if (matchedType == TokenType::kWhiteSpace) {
    } else if (matchedType == TokenType::kSepCloseBrace || matchedType == TokenType::kSepCloseParen) {
      checkBraceStack(braceStack, tokens, matchedType, matchedValue);
    } else if (matchedType == TokenType::kLiteralName) {
      checkValidLiteral(tokens, matchedType, matchedValue);
    } else {
      Token token{matchedType, matchedValue};
      tokens.push_back(token);
    }
  }
  if (!braceStack.empty()) {
    throw InvalidSyntaxError();
  }
  return tokens;
}

void SPtokeniser::checkBraceStack(std::stack<char>& braceStack,
                                  std::vector<Token>& tokens,
                                  TokenType matchedType,
                                  std::string matchedValue) {
  if (braceStack.empty()) {
    throw InvalidSyntaxError();
  } else {
    char top = braceStack.top();
    if (matchedValue[0] == '}' && top == '{') {
      braceStack.pop();
      Token token{matchedType, matchedValue};
      tokens.push_back(token);
    } else if (matchedValue[0] == ')' && top == '(') {
      braceStack.pop();
      // insert ')' as a token
      Token token{matchedType, matchedValue};
      tokens.push_back(token);
    } else {
      throw InvalidSyntaxError();
    }
  }
}

std::pair<TokenType, std::string> SPtokeniser::matchRegex(const std::string& line) {
  for (const auto& regex_rule : regex_rules) {
    std::regex regex_pattern = regex_rule.second;
    std::smatch match;

    // Iterate through defined regular expressions to match token types
    if (std::regex_search(line.begin(), line.end(), match, regex_pattern)) {
      return {regex_rule.first, match[0]};
    }
  }
  return {TokenType::kUnknownTokenType, ""};  // Return default values if no match is found
}

void SPtokeniser::checkValidLiteral(std::vector<Token>& tokens, TokenType matchedType, std::string matchedValue) {
  if (std::isdigit(matchedValue[0])) {
    throw InvalidTokenTypeError();
  } else {
    Token token{matchedType, matchedValue};
    tokens.push_back(token);
  }
}
