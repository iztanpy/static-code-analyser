#include "qps/query_parser/clause_builder/pattern_clause_builder.h"
#include "utils/string_utils.h"
#include "qps/query_parser/QueryUtil.h"

void PatternClauseBuilder::setPatternType(const QueryToken& token, const std::vector<Declaration>& declarations) {
  switch (token.type) {
    case PQLTokenType::PATTERN_IF:pattern_type = PatternType::kIf;
      break;
    case PQLTokenType::PATTERN_WHILE:pattern_type = PatternType::kWhile;
      break;
    case PQLTokenType::SYNONYM:pattern_type = PatternType::kAssignSyn;
      break;
    default:
      throw QpsSyntaxError("Invalid Pattern");
  }

  for (const Declaration& declaration : declarations) {
    if (declaration.synonym == token.text) {
      syn_assignment = declaration;
      break;
    }
  }
}

void PatternClauseBuilder::setLhs(const QueryToken& param, const std::vector<Declaration>& declarations) {
  switch (param.type) {
    case PQLTokenType::SYNONYM:
      for (const Declaration& declaration : declarations) {
        if (declaration.synonym == param.text) {
          lhs = declaration;
          break;
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

void PatternClauseBuilder::setRhs(const QueryToken& param, const std::vector<Declaration>& declarations) {
  switch (param.type) {
    // these 2 are cases for expression and partial expressions
    case PQLTokenType::EXACTEXPR:rhs = ExactExpr{string_util::RemoveSpacesFromExpr(param.text)};
      break;
    case PQLTokenType::PARTIALEXPR:rhs = PartialExpr{string_util::RemoveSpacesFromExpr(param.text)};
      break;
    case PQLTokenType::WILDCARD:rhs = Wildcard::Value;
      break;
    default: throw QpsSyntaxError("Syntax error");
  }
}

std::unique_ptr<PatternClause> PatternClauseBuilder::getClause() const {
  switch (pattern_type) {
    case PatternType::kWhile:
      return std::make_unique<WhilePattern>(syn_assignment, lhs);
    case PatternType::kIf:
      return std::make_unique<IfPattern>(syn_assignment, lhs);
    case PatternType::kAssignSyn:
      if (std::holds_alternative<Wildcard>(rhs)
          || std::holds_alternative<ExactExpr>(rhs)
          || std::holds_alternative<PartialExpr>(rhs)) {
        return std::make_unique<AssignPattern>(syn_assignment, lhs, rhs);
      } else {
        throw QpsSyntaxError("Syntax error");
      }
    default:
      throw QpsSyntaxError("Invalid Pattern");
    }
}
