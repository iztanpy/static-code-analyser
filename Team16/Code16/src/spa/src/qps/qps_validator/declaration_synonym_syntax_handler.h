#pragma once

#include <set>
#include <memory>
#include <string>
#include "qps/qps_validator/qps_validator_handler.h"

class DeclarationSynonymSyntaxHandler: public QpsValidatorHandler {
 public:
  DeclarationSynonymSyntaxHandler() = default;
  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;
  void handle(std::string synonym) override;
};
