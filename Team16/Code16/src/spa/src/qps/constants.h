#pragma once

#include <regex>
#include <vector>
#include <string>

namespace qps_constants {
const char kOpenBracket = '(';
const char kCloseBracket = ')';
const char kComma = ',';
const char kEqual = '=';
const char kSemicolon = ';';
const std::vector<char> kSpecialTokens = {
    kOpenBracket,
    kCloseBracket,
    kComma,
    kEqual,
    kSemicolon
};

const char kWildcard = '_';

const std::regex kSuchThatClauseRegex("such that [A-Za-z]");
const std::regex kPatternClauseRegex("pattern [A-Za-z]");
const std::regex kOnlySuchThat("such that$");
const std::regex kOnlyPattern("pattern$");
}  // namespace qps_constants
