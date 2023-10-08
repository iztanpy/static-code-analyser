#pragma once

#include <memory>
#include <string>
#include "qps/qps_validator/qps_validator_handler.h"

class SelectStatementSyntaxHandler: public QpsValidatorHandler {
 public:
  SelectStatementSyntaxHandler() = default;
  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;
  void handle(std::string select_statement) override;
};
