#include <memory>
#include <vector>
#include <string>
#include <utility>
#include "qps/qps_validator/clause_args_syntax_handler.h"
#include "utils/string_utils.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "qps/query_parser/QueryUtil.h"
#include "qps/qps_validator/WithClauseSyntaxHandler.h"

void WithClauseSyntaxHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void WithClauseSyntaxHandler::handle(std::string clause) {
  std::vector<std::string> arguments = string_util::SplitStringBy('=', clause);
  if (arguments.size() != 2) {
    throw QpsSyntaxError("More than 2 arguments");
  }

  std::string lhs = arguments[0];
  std::string rhs = arguments[1];
  if (!QueryUtil::IsRef(lhs) || !QueryUtil::IsRef(rhs)) {
    throw QpsSyntaxError("LHS or RHS of with clause is not a ref");
  }
}
