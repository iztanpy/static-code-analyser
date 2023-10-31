#include "qps/query_parser/clause_builder/select_clause_builder.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"

SelectClauseBuilder::SelectClauseBuilder() = default;

void SelectClauseBuilder::setDeclaration(Declaration declaration, PQLTokenType token_type) {
  declaration_ = std::move(declaration);
  attr_name_ = [&]() -> AttrName {
    switch (token_type) {
      case PQLTokenType::SYNONYM:return AttrName::NONE;
      case PQLTokenType::WITH_PROCNAME:return AttrName::PROCNAME;
      case PQLTokenType::WITH_VARNAME:return AttrName::VARNAME;
      case PQLTokenType::WITH_VALUE:return AttrName::VALUE;
      case PQLTokenType::WITH_STMTNO:return AttrName::STMTNUM;
      default:return AttrName::NONE;
    }
  }();
}

std::unique_ptr<SelectClause> SelectClauseBuilder::getClause() const {
  return std::make_unique<SelectClause>(AttrRef(declaration_, attr_name_));
}
