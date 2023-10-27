#pragma once

#include <memory>
#include <string>
#include "qps/qps_validator/qps_validator_handler.h"
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"

class WithClauseSyntaxHandler : public QpsValidatorHandler {
 public:
  WithClauseSyntaxHandler() = default;
  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;
  void handle(std::string clause) override;
};
