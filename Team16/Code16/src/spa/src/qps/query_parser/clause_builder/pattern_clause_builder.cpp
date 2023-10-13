#include "qps/query_parser/clause_builder/pattern_clause_builder.h"
#include "utils/string_utils.h"

void PatternClauseBuilder::setSynAssignment(Declaration declaration) {
  syn_assignment = std::move(declaration);
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
  rhs_type = param.type;
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
  if (rhs_type == PQLTokenType::WILDCARD || rhs_type == PQLTokenType::EXACTEXPR
      || rhs_type == PQLTokenType::PARTIALEXPR) {
    return std::make_unique<AssignPattern>(syn_assignment, lhs, rhs);
  } else {
    throw QpsSyntaxError("Syntax error");
  }
}
