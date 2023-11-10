#include <memory>
#include <utility>
#include <string>
#include <vector>
#include "qps/qps_validator/clause_synonym_semantic_handler.h"
#include "qps/query_parser/QueryUtil.h"

ClauseSynonymSemanticHandler::ClauseSynonymSemanticHandler(std::vector<Declaration> & declarations,
                                                           std::vector<QpsSemanticError> & semantic_errors)
    : declarations(declarations), semantic_errors(semantic_errors) {}

void ClauseSynonymSemanticHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void ClauseSynonymSemanticHandler::handle(std::string synonym) {
  if (!QueryUtil::IsInDeclarations(synonym, declarations)) {
    semantic_errors.emplace_back("Synonym not declared");
  }
}
