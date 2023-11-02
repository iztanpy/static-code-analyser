#include "qps/query_parser/clause_builder/with_clause_builder.h"
#include "qps/qps_errors/qps_syntax_error.h"

WithClauseBuilder::WithClauseBuilder() = default;

void WithClauseBuilder::setLhs(const QueryToken& param, const std::vector<Declaration>& declarations) {
  switch (param.type) {
    case PQLTokenType::INTEGER:lhs = std::stoi(param.text);
      break;
    case PQLTokenType::IDENT:lhs = param.text;
      break;
    case PQLTokenType::WITH_PROCNAME:
      for (const Declaration& declaration : declarations) {
        if (param.text == declaration.synonym) {
          AttrRef lhs_attr_ref = {declaration, AttrName::PROCNAME};
          lhs = lhs_attr_ref;
          break;
        }
      }
      break;
    case PQLTokenType::WITH_VARNAME:
      for (const Declaration& declaration : declarations) {
        if (param.text == declaration.synonym) {
          AttrRef lhs_attr_ref = {declaration, AttrName::VARNAME};
          lhs = lhs_attr_ref;
          break;
        }
      }
      break;
    case PQLTokenType::WITH_VALUE:
      for (const Declaration& declaration : declarations) {
        if (param.text == declaration.synonym) {
          AttrRef lhs_attr_ref = {declaration, AttrName::VALUE};
          lhs = lhs_attr_ref;
          break;
        }
      }
      break;
    case PQLTokenType::WITH_STMTNO:
      for (const Declaration& declaration : declarations) {
        if (param.text == declaration.synonym) {
          AttrRef lhs_attr_ref = {declaration, AttrName::STMTNUM};
          lhs = lhs_attr_ref;
          break;
        }
      }
      break;
    default:
      throw QpsSyntaxError("Not a with clause token type");
  }
}

void WithClauseBuilder::setRhs(const QueryToken& param, const std::vector<Declaration>& declarations) {
  switch (param.type) {
    case PQLTokenType::INTEGER:rhs = std::stoi(param.text);
      break;
    case PQLTokenType::IDENT:rhs = param.text;
      break;
    case PQLTokenType::WITH_PROCNAME:
      for (const Declaration& declaration : declarations) {
        if (param.text == declaration.synonym) {
          AttrRef rhs_attr_ref = {declaration, AttrName::PROCNAME};
          rhs = rhs_attr_ref;
          break;
        }
      }
      break;
    case PQLTokenType::WITH_VARNAME:
      for (const Declaration& declaration : declarations) {
        if (param.text == declaration.synonym) {
          AttrRef rhs_attr_ref = {declaration, AttrName::VARNAME};
          rhs = rhs_attr_ref;
          break;
        }
      }
      break;
    case PQLTokenType::WITH_VALUE:
      for (const Declaration& declaration : declarations) {
        if (param.text == declaration.synonym) {
          AttrRef rhs_attr_ref = {declaration, AttrName::VALUE};
          rhs = rhs_attr_ref;
          break;
        }
      }
      break;
    case PQLTokenType::WITH_STMTNO:
      for (const Declaration& declaration : declarations) {
        if (param.text == declaration.synonym) {
          AttrRef rhs_attr_ref = {declaration, AttrName::STMTNUM};
          rhs = rhs_attr_ref;
          break;
        }
      }
      break;
    default:
      throw QpsSyntaxError("Not a with clause token type");
  }
}

std::unique_ptr<WithClause> WithClauseBuilder::getClause() const {
  return std::make_unique<WithClause>(lhs, rhs, is_not);
}
