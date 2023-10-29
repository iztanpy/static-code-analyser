#include <vector>
#include <memory>
#include <utility>
#include <string>
#include "utils/lexical_utils.h"
#include "qps/qps_validator/select_synonym_semantic_handler.h"
#include "qps/query_parser/QueryUtil.h"

SelectSynonymSemanticHandler::SelectSynonymSemanticHandler(std::vector<Declaration> & declarations)
  : declarations(declarations) {}

void SelectSynonymSemanticHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void SelectSynonymSemanticHandler::handle(std::string select_synonym) {
  if (!QueryUtil::IsInDeclarations(select_synonym, declarations) && select_synonym != "BOOLEAN") {
    throw QpsSemanticError("Select synonym has not been declared");
  }
  if (next) {
    next->handle(select_synonym);
  }
}




