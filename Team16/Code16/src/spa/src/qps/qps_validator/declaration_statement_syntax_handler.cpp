#include <memory>
#include <utility>
#include "qps/qps_validator/declaration_statement_syntax_handler.h"
#include "utils/string_utils.h"
#include "qps/query_parser/QueryUtil.h"
#include "qps/qps_errors/qps_syntax_error.h"

DeclarationStatementSyntaxHandler::DeclarationStatementSyntaxHandler() = default;

void DeclarationStatementSyntaxHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void DeclarationStatementSyntaxHandler::handle(std::string declaration_statement) {
  std::string entity = string_util::GetFirstWord(declaration_statement);
  if (!QueryUtil::IsDesignEntity(entity)) {
    throw QpsSyntaxError("Design entity is not valid");
  }

  declaration_statement = string_util::RemoveFirstWord(declaration_statement);
  if (declaration_statement.empty()) {
    throw QpsSyntaxError("Missing synonyms for design entity");
  }
}
