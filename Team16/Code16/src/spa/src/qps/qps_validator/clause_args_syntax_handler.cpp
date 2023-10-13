#include <memory>
#include <vector>
#include <string>
#include <utility>
#include "qps/qps_validator/clause_args_syntax_handler.h"
#include "utils/string_utils.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "qps/query_parser/QueryUtil.h"

void ClauseArgsSyntaxHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void ClauseArgsSyntaxHandler::handle(std::string clause) {
  if (!QueryUtil::IsEnclosedInBrackets(clause)) {
    throw QpsSyntaxError("Missing brackets");
  }
  std::string clause_with_brackets_removed = QueryUtil::RemoveBrackets(clause);
  std::vector<std::string> arguments = string_util::SplitStringBy(',', clause_with_brackets_removed);
  if (arguments.size() != 2) {
    throw QpsSyntaxError("More than 2 arguments");
  }
}

void ClauseArgsSyntaxHandler::handle(const std::string & left_hand_side, const std::string & right_hand_side) {
  if (!QueryUtil::IsStmtRef(left_hand_side) && !QueryUtil::IsEntRef(left_hand_side)) {
    throw QpsSyntaxError("Invalid argument for relationship reference");
  }
  if (!QueryUtil::IsStmtRef(right_hand_side) && !QueryUtil::IsEntRef(right_hand_side)) {
    throw QpsSyntaxError("Invalid argument for relationship reference");
  }
}

void ClauseArgsSyntaxHandler::handle(std::string clause, PQLTokenType pattern_type) {
  if (!QueryUtil::IsEnclosedInBrackets(clause)) {
    throw QpsSyntaxError("Missing brackets");
  }
  std::string clause_with_brackets_removed = QueryUtil::RemoveBrackets(clause);
  std::vector<std::string> arguments = string_util::SplitStringBy(',', clause_with_brackets_removed);
  if (pattern_type == PQLTokenType::PATTERN_IF) {
    if (arguments.size() != 3) {
      throw QpsSyntaxError("More than 3 arguments");  // for if pattern
    }
  } else {
    if (arguments.size() != 2) {
      throw QpsSyntaxError("More than 2 arguments");  // for while and assign pattern
    }
  }
}
