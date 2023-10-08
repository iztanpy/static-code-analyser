#include <set>
#include "qps/qps_validator/declaration_synonym_semantic_handler.h"
#include "qps/qps_errors/qps_semantic_error.h"

DeclarationSynonymSemanticHandler::DeclarationSynonymSemanticHandler(std::set<std::string> & processed_synonyms)
  : processed_synonyms(processed_synonyms) {}

void DeclarationSynonymSemanticHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void DeclarationSynonymSemanticHandler::handle(std::string synonym) {
  if (processed_synonyms.find(synonym) != processed_synonyms.end()) {
    throw QpsSemanticError("Repeated synonym declaration");
  }
}
