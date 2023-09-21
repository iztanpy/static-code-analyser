#pragma once

#include <regex>

namespace qps_constants {
const std::string kOpenBracket = "(";
const std::string kCloseBracket = ")";
const std::string kComma = ",";
const std::string kEqual = "=";
const std::string kSemicolon = ";";
const std::vector<std::string> kSpecialTokens = {
    kOpenBracket,
    kCloseBracket,
    kComma,
    kEqual,
    kSemicolon
};

const std::string kWildcard = "_";

const std::regex kSuchThatClauseRegex("such that [A_Za-z]");
const std::regex kPatternClauseRegex("such that [A_Za-z]");
}
