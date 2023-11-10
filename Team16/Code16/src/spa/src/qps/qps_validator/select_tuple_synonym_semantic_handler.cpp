#include <vector>
#include <memory>
#include <utility>
#include <string>
#include "qps/qps_validator/select_tuple_synonym_semantic_handler.h"
#include "qps/query_parser/QueryUtil.h"

SelectTupleSynonymSemanticHandler::SelectTupleSynonymSemanticHandler(std::vector<Declaration> & declarations,
                                                                     std::vector<QpsSemanticError> & semantic_errors)
    : declarations(declarations), semantic_errors(semantic_errors) {}

void SelectTupleSynonymSemanticHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void SelectTupleSynonymSemanticHandler::handle(std::string select_synonym) {
  if (!QueryUtil::IsInDeclarations(select_synonym, declarations)) {
    semantic_errors.emplace_back("Select synonym has not been declared");
  }
  if (next) {
    next->handle(select_synonym);
  }
}




