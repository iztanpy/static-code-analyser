#pragma once

#include <string>

namespace lexical_utils {
bool IsLetter(char c);
bool IsDigit(char c);
bool IsNzDigit(char c);
bool IsIdent(std::string str);
bool IsName(std::string str);
bool IsInteger(std::string str);
}