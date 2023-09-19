#pragma once

#include <vector>
#include <memory>
#include <string>
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"
#include "qps/qps_errors/qps_syntax_error.h"

void PatternClauseBuilder::setSynAssignment(Declaration declaration) {
  declaration = declaration;
}

void PatternClauseBuilder::setLhs(const QueryToken & param, const std::vector<Declaration> & declarations) {
  switch (param.type) {
    case PQLTokenType::SYNONYM:
      for (const Declaration & declaration : declarations) {
        if (declaration.synonym == param.text) {
          lhs = declaration;
        }
      }
      break;
    case PQLTokenType::WILDCARD:lhs = Wildcard::Value;
      break;
    case PQLTokenType::IDENT:lhs = param.text;
      break;
    default:throw QpsSyntaxError("Syntax error");
  }
}

void PatternClauseBuilder::setRhs(const QueryToken & param, const std::vector<Declaration> & declarations) {
  rhs_type = param.type;
  switch (param.type) {
    // these 2 are cases for expression and partial expressions
    case PQLTokenType::IDENT:
    case PQLTokenType::PARTIALEXPR:rhs = param.text;  // e.g. "x + y" or _"x + y"
      break;
    case PQLTokenType::WILDCARD:rhs = Wildcard::Value;
      break;
    default: throw QpsSyntaxError("Syntax error");
  }
}
std::unique_ptr<PatternClause> PatternClauseBuilder::getClause() const {
  if (std::holds_alternative<Wildcard>(rhs) && rhs_type == PQLTokenType::WILDCARD) {
    return std::make_unique<WildCardPattern>(lhs, std::get<Wildcard>(rhs));
  } else if (std::holds_alternative<std::string>(rhs) && rhs_type == PQLTokenType::IDENT) {
    return std::make_unique<ExactPattern>(lhs, std::get<std::string>(rhs));
  } else if (std::holds_alternative<std::string>(rhs) && rhs_type == PQLTokenType::PARTIALEXPR) {
    return std::make_unique<PartialPattern>(lhs, std::get<std::string>(rhs));
  } else {
    throw QpsSyntaxError("Syntax error");
  }
}
