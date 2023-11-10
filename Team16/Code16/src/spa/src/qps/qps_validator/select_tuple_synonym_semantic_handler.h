#pragma once

#include <vector>
#include <memory>
#include <string>
#include "qps/qps_validator/qps_validator_handler.h"
#include "qps/declaration.h"

class SelectTupleSynonymSemanticHandler: public QpsValidatorHandler {
 public:
  std::vector<Declaration>& declarations;
  std::vector<QpsSemanticError> & semantic_errors;
  explicit SelectTupleSynonymSemanticHandler(std::vector<Declaration>& declarations,
                                             std::vector<QpsSemanticError> & semantic_errors);
  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;
  void handle(std::string select_synonym) override;
};
