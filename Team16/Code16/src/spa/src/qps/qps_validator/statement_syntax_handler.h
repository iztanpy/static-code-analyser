#pragma once

#include <string>
#include <memory>
#include "qps/qps_validator/qps_validator_handler.h"

class StatementSyntaxHandler: public QpsValidatorHandler {
 public:
  bool is_select_statement_processed;
  explicit StatementSyntaxHandler(bool is_select_statement_processed);

  /*!
   * Sets the next handler after this
   * @param handler is the next handler
   */
  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;

  /*!
   * Handles syntax validation of statements
   * @throws QpsSyntaxError exception if statement is syntactically invalid
   */
  void handle(std::string statement) override;
};
