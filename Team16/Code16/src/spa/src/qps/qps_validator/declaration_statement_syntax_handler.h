#pragma once

#include <string>
#include <memory>
#include "qps/qps_validator/qps_validator_handler.h"

class DeclarationStatementSyntaxHandler : public QpsValidatorHandler {
 public:
  DeclarationStatementSyntaxHandler();
  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;
  void handle(std::string declaration_statement) override;
};
