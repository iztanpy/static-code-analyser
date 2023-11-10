#pragma once

#include <memory>
#include <string>
#include <vector>
#include "qps/qps_validator/qps_validator_handler.h"
#include "qps/declaration.h"
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"

class PatternSynSemanticHandler: public QpsValidatorHandler {
 public:
  std::vector<Declaration> & declarations;
  std::vector<QpsSemanticError> & semantic_errors;
  explicit PatternSynSemanticHandler(std::vector<Declaration> & declarations,
                                     std::vector<QpsSemanticError> & semantic_errors);
  void setNext(std::unique_ptr<QpsValidatorHandler> handler) override;
  void handle(std::string synonym) override;
};

