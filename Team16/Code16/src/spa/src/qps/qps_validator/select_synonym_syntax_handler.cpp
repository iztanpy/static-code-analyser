#include <memory>
#include <utility>
#include <string>
#include "qps/qps_validator/select_synonym_syntax_handler.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "utils/lexical_utils.h"

void SelectSynonymSyntaxHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void SelectSynonymSyntaxHandler::handle(std::string select_synonym) {
  if (!lexical_utils::IsSynonym(select_synonym)) {
    throw QpsSyntaxError("Synonym does not follow lexical rules");
  }
  if (next) {
    next->handle(select_synonym);
  }
}

