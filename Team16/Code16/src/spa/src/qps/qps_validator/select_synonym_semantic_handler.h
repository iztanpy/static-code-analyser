#pragma once

#include <vector>
#include <memory>
#include <string>
#include "qps/qps_validator/qps_validator_handler.h"
#include "qps/declaration.h"

class SelectSynonymSemanticHandler: public QpsValidatorHandler {
 public:
  std::vector<Declaration>& declarations;
  explicit SelectSynonymSemanticHandler(std::vector<Declaration>& declarations);
  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;
  void handle(std::string select_synonym) override;
};
