#include <utility>
#include "utils/lexical_utils.h"
#include "utils/string_utils.h"

bool lexical_utils::IsLetter(char c) {
  return isalpha(c);
}

bool lexical_utils::IsDigit(char c) {
  return isdigit(c);
}

// ai-gen start (gpt3, 0)
bool lexical_utils::IsNzDigit(char c) {
  return (c >= '1' && c <= '9');
}
// ai-gen end

bool lexical_utils::IsIdent(std::string str) {
  str = string_util::Trim(str);
  bool starts_with_letter = IsLetter(str[0]);
  if (!starts_with_letter) {
    return false;
  }

  for (char c : str) {
    if (!IsDigit(c) && !IsLetter(c)) {
      return false;
    }
  }
  return true;
}

bool lexical_utils::IsSynonym(std::string str) {
  return lexical_utils::IsIdent(str);
}

// ai-gen start(gpt3, 1)
bool lexical_utils::IsFactor(std::string str) {
  if (str.empty()) {
    return false;
  }

  if (str.front() == '(' && str.back() == ')') {
    std::string innerExpr = str.substr(1, str.size() - 2);
    return IsExpr(innerExpr);
  }

  return IsName(str) || IsInteger(str);
}

bool lexical_utils::IsTerm(std::string str) {
  int len = str.length();
  int bracketCount = 0;

  for (int i = len - 1; i >= 0; --i) {
    char c = str[i];
    if (c == '(') {
      bracketCount++;
    } else if (c == ')') {
      bracketCount--;
      if (bracketCount > 0) {
        return false; // More closing brackets than opening brackets
      }
    }

    if (bracketCount == 0 && (c == '*' || c == '/' || c == '%')) {
      std::string left = str.substr(0, i);
      std::string right = str.substr(i + 1);
      return IsTerm(left) && IsFactor(right);
    }
  }

  return IsFactor(str);
}

bool lexical_utils::IsExpr(std::string str) {
  int len = str.length();
  int bracketCount = 0;

  for (int i = len - 1; i >= 0; --i) {
    char c = str[i];
    if (c == '(') {
      bracketCount++;
    } else if (c == ')') {
      bracketCount--;
      if (bracketCount > 0) {
        return false; // More closing brackets than opening brackets
      }
    }

    if (bracketCount == 0 && (c == '+' || c == '-')) {
      std::string left = str.substr(0, i);
      std::string right = str.substr(i + 1);
      return IsExpr(left) && IsTerm(right);
    }
  }

  return IsTerm(str);
}
// ai-gen end

bool lexical_utils::IsName(std::string str) {
  str = string_util::Trim(str);
  return lexical_utils::IsIdent(std::move(str));  // they share the same rules
}

bool lexical_utils::IsInteger(std::string str) {
  str = string_util::Trim(str);
  if (str == "0") {
    return true;
  }

  bool starts_with_nz = lexical_utils::IsNzDigit(str[0]);
  if (!starts_with_nz) {
    return false;
  }

  for (char c : str) {
    if (!IsDigit(c)) {
      return false;
    }
  }
  return true;
}
