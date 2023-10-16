#include <memory>
#include <utility>
#include <string>
#include <vector>
#include "qps/qps_validator/clause_synonym_semantic_handler.h"
#include "qps/query_parser/QueryUtil.h"

ClauseSynonymSemanticHandler::ClauseSynonymSemanticHandler(std::vector<Declaration> & declarations)
    : declarations(declarations) {}

void ClauseSynonymSemanticHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void ClauseSynonymSemanticHandler::handle(std::string synonym) {
  if (!QueryUtil::IsInDeclarations(synonym, declarations)) {
    throw QpsSemanticError("Synonym not declared");
  }
//
//  if (!QueryUtil::IsSynAssign(synonym, declarations)
//      && !QueryUtil::IsSynIf(synonym, declarations)
//      && !QueryUtil::IsSynWhile(synonym, declarations)) {
//    throw QpsSemanticError("Invalid pattern type");
//  }
}
