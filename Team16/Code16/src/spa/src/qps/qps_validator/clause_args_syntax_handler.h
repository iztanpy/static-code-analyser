#pragma once

#include <memory>
#include <string>
#include "qps/qps_validator/qps_validator_handler.h"
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"

class ClauseArgsSyntaxHandler: public QpsValidatorHandler {
 public:
  ClauseArgsSyntaxHandler() = default;
  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;
  void handle(std::string clause) override;
  static void handle(const std::string & left_hand_side, const std::string & right_hand_side);
  static void handle(const std::string clause, PQLTokenType pattern_type);
};
