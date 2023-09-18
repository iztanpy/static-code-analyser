#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "TokenTypes.h"
#include "Token.h"
#include <regex>
#include <set>
#include <utility>
#include <stack>
#include <unordered_set>

class SPtokeniser {
 public:
    std::vector<std::string> splitLines(const std::string& input);
    std::vector<struct Token> tokenise(const std::string& input);
};
