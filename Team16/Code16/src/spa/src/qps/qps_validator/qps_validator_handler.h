#pragma once

#include <memory>
#include <string>

class QpsValidatorHandler {
 public:
  std::unique_ptr<QpsValidatorHandler> next;
  virtual void setNext(std::unique_ptr<QpsValidatorHandler> handler) = 0;
  virtual void handle(std::string request) = 0;
  virtual ~QpsValidatorHandler() = default;
};
