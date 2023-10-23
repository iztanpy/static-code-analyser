#pragma once

#include <memory>
#include <string>
#include <vector>
#include "qps/qps_validator/qps_validator_handler.h"
#include "qps/declaration.h"

class ClauseSynonymSemanticHandler: public QpsValidatorHandler {
 public:
  std::vector<Declaration> & declarations;
  explicit ClauseSynonymSemanticHandler(std::vector<Declaration> & declarations);
  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;
  void handle(std::string synonym) override;
};
