#include <vector>
#include <memory>
#include <utility>
#include <string>
#include "qps/qps_validator/select_synonym_semantic_handler.h"
#include "qps/query_parser/QueryUtil.h"

SelectSynonymSemanticHandler::SelectSynonymSemanticHandler(std::vector<Declaration> & declarations,
                                                           std::vector<QpsSemanticError> & semantic_errors)
  : declarations(declarations), semantic_errors(semantic_errors) {}

void SelectSynonymSemanticHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void SelectSynonymSemanticHandler::handle(std::string select_synonym) {
  if (!QueryUtil::IsInDeclarations(select_synonym, declarations) && select_synonym != "BOOLEAN") {
    semantic_errors.emplace_back("Select synonym has not been declared");
  }
  if (next) {
    next->handle(select_synonym);
  }
}




