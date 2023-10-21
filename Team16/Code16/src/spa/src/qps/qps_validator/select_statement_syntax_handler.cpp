#include <memory>
#include <utility>
#include <string>
#include "qps/qps_validator/select_statement_syntax_handler.h"
#include "utils/string_utils.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/constants.h"

void SelectStatementSyntaxHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void SelectStatementSyntaxHandler::handle(std::string select_statement) {
  // remove the Select keyword (guaranteed to exist since we already checked)
  std::string remaining_statement = string_util::RemoveFirstWord(select_statement);

  if (remaining_statement.empty()) {
    throw QpsSyntaxError("Missing select statement");
  }

  // remove the synonym and check if it can still be a such that / pattern clause
  std::string synonym_removed_statement = QueryTokenizer::removeSelectClause(remaining_statement);
  bool statement_can_match_clause =
      QueryTokenizer::clauseMatch(synonym_removed_statement, qps_constants::kSuchThatClauseRegex) ||
          QueryTokenizer::clauseMatch(synonym_removed_statement, qps_constants::kPatternClauseRegex);

  if (!synonym_removed_statement.empty() && !statement_can_match_clause) {
    // it is syntactically invalid
    throw QpsSyntaxError("Missing select synonym");
  }

  if (next) {
    next->handle(select_statement);
  }
}
