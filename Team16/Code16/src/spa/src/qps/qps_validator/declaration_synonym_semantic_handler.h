#pragma once

#include <set>
#include <memory>
#include <utility>
#include <string>
#include <vector>
#include "qps/qps_validator/qps_validator_handler.h"
#include "qps/qps_errors/qps_semantic_error.h"

class DeclarationSynonymSemanticHandler: public QpsValidatorHandler {
 public:
  std::set<std::string> & processed_synonyms;
  std::vector<QpsSemanticError> & semantic_errors;
  explicit DeclarationSynonymSemanticHandler(std::set<std::string> & processed_synonyms,
                                             std::vector<QpsSemanticError> & semantic_errors);

  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;
  void handle(std::string synonym) override;
};
