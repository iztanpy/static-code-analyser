#pragma once

#include <memory>
#include <string>
#include "qps/qps_validator/qps_validator_handler.h"

class SelectSynonymSyntaxHandler: public QpsValidatorHandler {
 public:
  SelectSynonymSyntaxHandler() = default;
  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;
  void handle(std::string select_synonym) override;
};
