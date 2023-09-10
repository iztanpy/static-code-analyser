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

class SPtokeniser {
 private:
    std::vector<std::string> splitLines(const std::string& input);
 public:
    std::vector<struct Token> tokenise(const std::string& input);
};
