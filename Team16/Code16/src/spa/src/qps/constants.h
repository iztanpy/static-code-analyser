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
const char kWildcard = '_';
const std::string kNot = "not";

const std::vector<char> kSpecialTokens = {
    kOpenBracket,
    kCloseBracket,
    kComma,
    kEqual,
    kSemicolon
};

const std::string kSelectKeyword = "Select";

const std::regex kSuchThatClauseRegex("such that [A-Za-z]");
const std::regex kPatternClauseRegex("pattern [A-Za-z]");
const std::regex kAndClauseRegex("and [\"A-Za-z0-9]");
const std::regex kWithClauseRegex("with [\"A-Za-z0-9]");
const std::regex kOnlySuchThat("such that$");
const std::regex kOnlyPattern("pattern$");
const std::regex kOnlyAnd("and$");
const std::regex kOnlyWith("with$");

const std::regex kTupleRegex("<[^>]+>");
}  // namespace qps_constants
