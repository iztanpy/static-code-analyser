#pragma once

#include <regex>
#include <sstream>
#include "string_utils.h"

// ai-gen start(gpt3, 1)
std::string string_util::LeftTrim(const std::string & str) {
  std::regex r("^\\s+");
  return std::regex_replace(str, r, std::string(""));
}

std::string string_util::RightTrim(const std::string & str) {
  std::regex r("\\s+$");
  return std::regex_replace(str, r, std::string(""));
}

std::string string_util::Trim(const std::string & str) {
  return RightTrim(LeftTrim(str));
}
// ai-gen end

/*
 * Reference: https://stackoverflow.com/questions/35301432/remove-extra-white-spaces-in-c
 */
std::string string_util::RemoveWhiteSpaces(const std::string & str) {
  std::string trimmedStr;
  std::unique_copy(str.begin(), str.end(), std::back_insert_iterator<std::string>(trimmedStr),
                   [](char a, char b) { return isspace(a) && isspace(b); });
  return Trim(trimmedStr);
}

std::vector<std::string> string_util::SplitStringBy(const char & delimiter, std::string & str) {
  std::vector<std::string> result;
  std::string token;
  std::istringstream tokenizer(str);

  while (std::getline(tokenizer, token, delimiter)) {
    result.push_back(Trim(token));
  }
  return result;
}

// ai-gen start (gpt3, 0)
std::string string_util::GetFirstWord(std::string & str) {
  std::istringstream iss(str);
  std::string firstWord;
  iss >> firstWord;
  return firstWord;
}

std::string string_util::RemoveFirstWord(std::string & str) {
  // Find the position of the first space character
  size_t pos = str.find(' ');

  if (pos != std::string::npos) {
    // Use substring to extract the portion of the string after the first space
    return str.substr(pos + 1);
  }

  // If there's no space, return an empty string
  return "";
}
//ai-gen end