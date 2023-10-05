
#include <memory>
#include <utility>
#include <string>
#include "qps/qps_validator/declaration_synonym_syntax_handler.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "utils/lexical_utils.h"

void DeclarationSynonymSyntaxHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void DeclarationSynonymSyntaxHandler::handle(std::string synonym) {
  if (!lexical_utils::IsIdent(synonym)) {
    throw QpsSyntaxError("Synonym does not follow lexical rules");
  }
  if (next) {
    next->handle(synonym);
  }
}
