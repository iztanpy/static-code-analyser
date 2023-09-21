#pragma once

#include "lexical_utils.h"

#include <utility>

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
//ai-gen end

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