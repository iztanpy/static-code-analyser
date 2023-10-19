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
#include "qps/qps_validator/clause_args_syntax_handler.h"
#include "qps/qps_validator/clause_synonym_semantic_handler.h"
#include "qps/query_parser/QueryUtil.h"
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"
#include "qps/qps_validator/pattern_syn_semantic_handler.h"
#include "utils/string_utils.h"
#include "qps/constants.h"

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

void qps_validator::ValidateClauseIndexes(std::vector<size_t> & indexes) {
  if (indexes.empty()) {
    // the clauses are wrong syntactically
    throw QpsSyntaxError("Invalid clause expressions");
  }
  if (indexes[0] > 0) {
    // clause does not start immediately after Select clause e.g., Select v ____ such that _____
    throw QpsSyntaxError("Unexpected clause expression");
  }
}

void qps_validator::ValidateClauseArgs(std::string clause) {
  ClauseArgsSyntaxHandler syntax_handler = ClauseArgsSyntaxHandler();
  syntax_handler.handle(std::move(clause));
}

void qps_validator::ValidateClauseArgs(std::string clause, PQLTokenType pattern_type) {
  ClauseArgsSyntaxHandler syntax_handler = ClauseArgsSyntaxHandler();
  syntax_handler.handle(std::move(clause), pattern_type);
}

void qps_validator::ValidateClauseArgs(std::string lhs, std::string rhs) {
  ClauseArgsSyntaxHandler syntax_handler = ClauseArgsSyntaxHandler();
  syntax_handler.handle(lhs, rhs);
}

void qps_validator::ValidateClauseSynonym(std::string synonym, std::vector<Declaration> & declarations) {
  ClauseSynonymSemanticHandler semantic_handler = ClauseSynonymSemanticHandler(declarations);
  semantic_handler.handle(std::move(synonym));
}

void qps_validator::ValidatePatternSynonym(std::string pattern_syn, std::vector<Declaration> & declarations) {
  PatternSynSemanticHandler semantic_handler = PatternSynSemanticHandler(declarations);
  semantic_handler.handle(pattern_syn);
}

void qps_validator::ValidatePatternClauseArgs(const std::string& left_hand_side,
                                              const std::string& right_hand_side,
                                              PQLTokenType pattern_type) {
  // All pattern LHS are entRef
  if (!QueryUtil::IsEntRef(left_hand_side)) {
    throw QpsSyntaxError("Invalid argument for LHS pattern clause");
  }

  // RHS can only be these 3 types regardless of pattern synonym
  if (!QueryUtil::IsPartialMatchExpressionSpecification((right_hand_side))
      && !QueryUtil::IsExactExpressionSpecification(right_hand_side)
      && !QueryUtil::IsWildcard(right_hand_side)) {
    throw QpsSyntaxError("Invalid argument for RHS pattern clause");
  }

  if (QueryUtil::IsPartialMatchExpressionSpecification((right_hand_side))
      || QueryUtil::IsExactExpressionSpecification(right_hand_side)) {
    if (pattern_type == PQLTokenType::PATTERN_IF) {
      throw QpsSyntaxError("Invalid if pattern syntax");
    } else if (pattern_type == PQLTokenType::PATTERN_WHILE) {
      throw QpsSemanticError("Pattern synonym must be an assign synonym");  // because can only be wildcard
    }
  }
}

void qps_validator::ValidateIfPatternClause(std::vector<std::string>& arguments) {
  if (arguments.size() != 3) {
    throw QpsSemanticError("Pattern synonym is not if statement");
  }
  std::string third_token = string_util::RemoveSpacesFromExpr(arguments[2]);
  if (!QueryUtil::IsWildcard(third_token)) {
    throw QpsSyntaxError("Third argument of if pattern is not wildcard");
  }
}

void qps_validator::ValidateNonEmptyClause(const std::string & clause_with_keyword_removed) {
  if (clause_with_keyword_removed.empty()) {
    throw QpsSyntaxError("Missing input after such that");
  }
}

void qps_validator::ValidateRelRef(const std::string & rel_ref) {
  if (!QueryUtil::IsRelRef(rel_ref)) {
    throw QpsSyntaxError("Invalid relation reference type");
  }
}

void qps_validator::ValidateAndIsNotFirstClause(ClauseEnum prev_clause) {
  if (prev_clause == ClauseEnum::NONE) {
    throw QpsSyntaxError("Missing clause before and");
  }
}

void qps_validator::ValidateAndClause(std::string& curr_clause) {
  if (QueryTokenizer::clauseMatch(curr_clause, qps_constants::kSuchThatClauseRegex)
      ||QueryTokenizer::clauseMatch(curr_clause, qps_constants::kPatternClauseRegex)) {
    throw QpsSyntaxError("And is followed by such that or pattern");
  }
}
