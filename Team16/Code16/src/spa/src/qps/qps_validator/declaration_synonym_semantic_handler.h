#pragma once

#include <set>
#include <memory>
#include <utility>
#include <string>
#include "qps/qps_validator/qps_validator_handler.h"

class DeclarationSynonymSemanticHandler: public QpsValidatorHandler {
 public:
  std::set<std::string> & processed_synonyms;
  explicit DeclarationSynonymSemanticHandler(std::set<std::string> & processed_synonyms);

  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;
  void handle(std::string synonym) override;
};
