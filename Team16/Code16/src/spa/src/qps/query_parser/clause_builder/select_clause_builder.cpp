#include "qps/query_parser/clause_builder/select_clause_builder.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"

SelectClauseBuilder::SelectClauseBuilder() = default;

void SelectClauseBuilder::setDeclaration(Declaration declaration, PQLTokenType token_type) {
  selectClause.declaration = std::move(declaration);
  switch (token_type) {
    case PQLTokenType::SYNONYM: selectClause.attr_name = AttrName::NONE;
      break;
    case PQLTokenType::WITH_PROCNAME: selectClause.attr_name = AttrName::PROCNAME;
      break;
    case PQLTokenType::WITH_VARNAME: selectClause.attr_name = AttrName::VARNAME;
      break;
    case PQLTokenType::WITH_VALUE: selectClause.attr_name = AttrName::VALUE;
      break;
    case PQLTokenType::WITH_STMTNO: selectClause.attr_name = AttrName::STMTNUM;
      break;
    default: selectClause.attr_name = AttrName::NONE;
      break;
  }
}

std::unique_ptr<SelectClause> SelectClauseBuilder::getClause() const {
  return std::make_unique<SelectClause>(selectClause);
}
