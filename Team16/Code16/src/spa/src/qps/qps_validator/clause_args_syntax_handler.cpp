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
  std::vector<std::string> synonyms = string_util::SplitStringBy(',', clause);
  if (synonyms.size() != 2) {
    throw QpsSyntaxError("More than 2 synonyms in relation reference");
  }

  std::vector<std::string> lhs = string_util::SplitStringBy('(', synonyms[0]);
  if (lhs.size() != 2) {
    throw QpsSyntaxError("More than 2 open brackets or missing arguments");
  }
  std::vector<std::string> rhs = string_util::SplitStringBy(')', synonyms[1]);
  if (rhs.size() != 2) {
    throw QpsSyntaxError("More than 2 close brackets or missing arguments");
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
