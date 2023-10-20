#include <vector>
#include <memory>
#include <utility>
#include <string>
#include "qps/qps_validator/select_tuple_synonym_semantic_handler.h"
#include "qps/query_parser/QueryUtil.h"

SelectTupleSynonymSemanticHandler::SelectTupleSynonymSemanticHandler(std::vector<Declaration> & declarations)
    : declarations(declarations) {}

void SelectTupleSynonymSemanticHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void SelectTupleSynonymSemanticHandler::handle(std::string select_synonym) {
  if (!QueryUtil::IsInDeclarations(select_synonym, declarations)) {
    throw QpsSemanticError("Select synonym has not been declared");
  }
  if (next) {
    next->handle(select_synonym);
  }
}




