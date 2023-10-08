#include <memory>
#include <utility>
#include "qps/qps_validator/statement_syntax_handler.h"
#include "qps/qps_errors/qps_syntax_error.h"

StatementSyntaxHandler::StatementSyntaxHandler(bool is_select_statement_processed)
: is_select_statement_processed(is_select_statement_processed) {}

void StatementSyntaxHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void StatementSyntaxHandler::handle(std::string statement) {
  if (is_select_statement_processed) {
    if (statement.empty()) {
      throw QpsSyntaxError("Extra character at the end of query");
    }
    throw QpsSyntaxError("Statements after select statement are invalid");
  }
}
