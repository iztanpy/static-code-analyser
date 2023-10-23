#include <utility>
#include "qps/qps_validator/pattern_syn_semantic_handler.h"
#include "qps/query_parser/QueryUtil.h"

PatternSynSemanticHandler::PatternSynSemanticHandler(std::vector<Declaration> & declarations)
  :declarations(declarations) {}

void PatternSynSemanticHandler::setNext(std::unique_ptr<QpsValidatorHandler> handler) {
  this->next = std::move(handler);
}

void PatternSynSemanticHandler::handle(std::string synonym) {
  if (!QueryUtil::IsSynonym(synonym)) {
    throw QpsSyntaxError("Invalid synonym");
  }

  if (!QueryUtil::IsSynAssign(synonym, declarations)
      && !QueryUtil::IsSynIf(synonym, declarations)
      && !QueryUtil::IsSynWhile(synonym, declarations)) {
    throw QpsSemanticError("Invalid pattern type");
  }
}
