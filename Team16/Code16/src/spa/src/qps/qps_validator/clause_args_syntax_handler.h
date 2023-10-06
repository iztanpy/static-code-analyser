#pragma once

#include <memory>
#include <string>
#include "qps/qps_validator/qps_validator_handler.h"

class ClauseArgsSyntaxHandler: public QpsValidatorHandler {
 public:
  ClauseArgsSyntaxHandler() = default;
  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;
  void handle(std::string clause) override;
  void handle(const std::string & left_hand_side, const std::string & right_hand_side);
};
