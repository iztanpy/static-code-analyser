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
  // Check if the string is a factor (var_name, const_value, or '(' expr ')')
  return IsName(str) || IsInteger(str) || (
          (str.size() >= 2 && str.front() == '(' && str.back() == ')'
          && IsExpr(str.substr(1, str.size() - 2))));
}

bool lexical_utils::IsTerm(std::string str) {
  if (IsFactor(str)) {
    return true;
  } else {
    // Check if the string is a term (term '*' factor, term '/' factor, term '%' factor, or factor)
    size_t pos = str.find_last_of("*/%");
    if (pos != std::string::npos) {
      std::string left = str.substr(0, pos);
      std::string right = str.substr(pos + 1);
      return IsTerm(left) && IsFactor(right);
    } else {
      return IsFactor(str);
    }
  }
}

bool lexical_utils::IsExpr(std::string str) {
  if (IsTerm(str)) {
    return true;
  } else {
    // Check if the string is an expression (expr '+' term, expr '-' term, term)
    size_t pos = str.find_last_of("+-");
    if (pos != std::string::npos) {
      std::string left = str.substr(0, pos);
      std::string right = str.substr(pos + 1);
      return IsExpr(left) && IsTerm(right);
    } else {
      return IsTerm(str);
    }
  }
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
