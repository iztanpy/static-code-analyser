#include <regex>
#include <sstream>
#include <vector>
#include "utils/string_utils.h"

// ai-gen start(gpt3, 1)
std::string string_util::LeftTrim(const std::string& str) {
  std::regex r("^\\s+");
  return std::regex_replace(str, r, std::string(""));
}

std::string string_util::RightTrim(const std::string& str) {
  std::regex r("\\s+$");
  return std::regex_replace(str, r, std::string(""));
}

std::string string_util::Trim(const std::string& str) {
  return RightTrim(LeftTrim(str));
}

bool string_util::IsWhiteSpace(const std::string& str) {
  for (char c : str) {
    if (!std::isspace(static_cast<unsigned char>(c))) {
      return false;
    }
  }
  return true;
}
// ai-gen end

/*
 * Reference: https://stackoverflow.com/questions/35301432/remove-extra-white-spaces-in-c
 */
std::string string_util::RemoveWhiteSpaces(const std::string& str) {
  std::string trimmedStr;
  std::unique_copy(str.begin(), str.end(), std::back_insert_iterator<std::string>(trimmedStr),
                   [](char a, char b) { return isspace(a) && isspace(b); });
  return Trim(trimmedStr);
}

std::vector<std::string> string_util::SplitStringBy(const char& delimiter, std::string& str) {
  std::vector<std::string> result;
  std::string token;
  std::istringstream tokenizer(str);

  while (std::getline(tokenizer, token, delimiter)) {
    result.push_back(Trim(token));
  }

  // a check for extra characters at the end
  if (token.empty()) {
    result.emplace_back("");
  }

  return result;
}

// ai-gen start (gpt3, 0)
std::string string_util::GetFirstWord(std::string& str) {
  std::istringstream iss(str);
  std::string firstWord;
  iss >> firstWord;
  return firstWord;
}

std::string string_util::GetFirstWordFromArgs(std::string& str) {
  std::vector<std::string> split_args = string_util::SplitStringBy('(', str);
  return string_util::RemoveWhiteSpaces(split_args[0]);
}

std::string string_util::RemoveFirstWordFromArgs(std::string& str) {
  // Find the position of the first ( character
  size_t pos = str.find('(');

  if (pos != std::string::npos) {
    // Use substring to extract the portion of the string after the first space
    return str.substr(pos);
  }

  // If there's no space, return an empty string
  return "";
}

std::string string_util::RemoveFirstWord(std::string& str) {
  // Find the position of the first space character
  size_t pos = str.find(' ');

  if (pos != std::string::npos) {
    // Use substring to extract the portion of the string after the first space
    return str.substr(pos + 1);
  }

  // If there's no space, return an empty string
  return "";
}

std::string string_util::RemoveSpacesFromExpr(const std::string& str) {
  std::string result;
  for (char c : str) {
    if (!std::isspace(c)) {
      result += c;
    }
  }
  return result;
}
// ai-gen end
