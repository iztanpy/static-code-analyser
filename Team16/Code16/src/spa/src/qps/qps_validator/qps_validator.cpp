#include <utility>
#include <set>
#include <vector>
#include <memory>
#include "qps/qps_validator/qps_validator.h"
#include "qps/qps_validator/statement_syntax_handler.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "qps/qps_validator/declaration_statement_syntax_handler.h"
#include "qps/qps_validator/declaration_synonym_syntax_handler.h"
#include "qps/qps_validator/declaration_synonym_semantic_handler.h"
#include "qps/qps_validator/select_statement_syntax_handler.h"
#include "qps/qps_validator/select_synonym_syntax_handler.h"
#include "qps/qps_validator/select_synonym_semantic_handler.h"

void qps_validator::ValidateStatement(std::string statement, bool is_select_statement_processed) {
  StatementSyntaxHandler statement_syntax_handler = StatementSyntaxHandler(is_select_statement_processed);
  statement_syntax_handler.handle(std::move(statement));
}

void qps_validator::ValidateSelectExists(const std::string& select_statement) {
  if (select_statement.empty()) {
    throw QpsSyntaxError("No select statement");
  }
}

void qps_validator::ValidateDeclarationStatement(std::string statement) {
  DeclarationStatementSyntaxHandler handler = DeclarationStatementSyntaxHandler();
  handler.handle(std::move(statement));
}

void qps_validator::ValidateDeclarationSynonym(std::string synonym, std::set<std::string> & processed_synonyms) {
  DeclarationSynonymSyntaxHandler syntax_handler = DeclarationSynonymSyntaxHandler();
  std::unique_ptr<QpsValidatorHandler> semantic_handler(new DeclarationSynonymSemanticHandler(processed_synonyms));
  syntax_handler.setNext(std::move(semantic_handler));
  syntax_handler.handle(std::move(synonym));
}

void qps_validator::ValidateSelectStatement(std::string select_statement) {
  SelectStatementSyntaxHandler handler = SelectStatementSyntaxHandler();
  handler.handle(std::move(select_statement));
}

void qps_validator::ValidateSelectSynonym(std::string select_synonym, std::vector<Declaration> & declarations) {
  SelectSynonymSyntaxHandler syntax_handler = SelectSynonymSyntaxHandler();
  std::unique_ptr<QpsValidatorHandler> semantic_handler(new SelectSynonymSemanticHandler(declarations));
  syntax_handler.setNext(std::move(semantic_handler));
  syntax_handler.handle(std::move(select_synonym));
}

void qps_validator::ValidateClauseIndexes(std::vector<size_t>& indexes) {
  if (indexes.empty()) {
    // the clauses are wrong syntactically
    throw QpsSyntaxError("Invalid clause expressions");
  }
  if (indexes[0] > 0) {
    // clause does not start immediately after Select clause e.g., Select v ____ such that _____
    throw QpsSyntaxError("Unexpected clause expression");
  }
}
