#pragma once

#include <utility>
#include "utils/lexical_utils.h"

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
// TODO(Su Mian): need to check for complex expressions
// bool lexical_utils::IsExpr(std::string str) {
//  std::vector<std::string> split_by_plus = string_util::SplitStringBy('+', str);
//  std::vector<std::string> split_by_minus = string_util::SplitStringBy('-', str);
//  if (split_by_minus.size() == 1 && split_by_plus.size() == 1) {
//    // expression does not have + or -
//    return IsTerm(str);
//  }
//
//}
//
// bool lexical_utils::IsFactor(std::string str) {
//  if (str[0] == qps_constants::kOpenBracket && str[str.length() - 1] == qps_constants::kCloseBracket) {
//    return IsExpr(str.substr(1, str.length() - 2));
//  } else {
//    return IsName(str) || IsInteger(str);
//  }
//}

bool lexical_utils::IsName(std::string str) {
  return lexical_utils::IsIdent(std::move(str));  // they share the same rules
}

bool lexical_utils::IsInteger(std::string str) {
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
