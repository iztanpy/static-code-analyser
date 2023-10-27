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
#include "qps/qps_validator/select_tuple_synonym_semantic_handler.h"
#include "WithClauseSyntaxHandler.h"

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

void qps_validator::ValidateWithClauseArgs(std::string & with_clause) {
  WithClauseSyntaxHandler syntax_handler = WithClauseSyntaxHandler();
  syntax_handler.handle(with_clause);
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

  std::string processed_right_hand_side = string_util::RemoveSpacesFromExpr(right_hand_side);
  // RHS can only be these 3 types regardless of pattern synonym
  if (!QueryUtil::IsPartialMatchExpressionSpecification(processed_right_hand_side)
      && !QueryUtil::IsExactExpressionSpecification(processed_right_hand_side)
      && !QueryUtil::IsWildcard(processed_right_hand_side)) {
    throw QpsSyntaxError("Invalid argument for RHS pattern clause");
  }

  if (QueryUtil::IsPartialMatchExpressionSpecification((processed_right_hand_side))
      || QueryUtil::IsExactExpressionSpecification(processed_right_hand_side)) {
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
      ||QueryTokenizer::clauseMatch(curr_clause, qps_constants::kPatternClauseRegex)
      || QueryTokenizer::clauseMatch(curr_clause, qps_constants::kWithClauseRegex)) {
    throw QpsSyntaxError("And is followed by such that, pattern or with");
  }
}

void qps_validator::ValidateSelectTuple(std::string& select_value, std::vector<Declaration> & declarations) {
  std::string select_value_with_tuple_removed = QueryUtil::RemoveTuple(select_value);
  std::vector<std::string> tuple_arguments = string_util::SplitStringBy(',', select_value_with_tuple_removed);
  SelectSynonymSyntaxHandler syntax_handler = SelectSynonymSyntaxHandler();
  std::unique_ptr<QpsValidatorHandler> semantic_handler(new SelectTupleSynonymSemanticHandler(declarations));
  syntax_handler.setNext(std::move(semantic_handler));
  for (const std::string& argument : tuple_arguments) {
    if (argument.empty()) {
      throw QpsSyntaxError("Missing argument in tuple");
    }
    syntax_handler.handle(argument);
  }
}

void qps_validator::ValidateSelectValue(std::string & select_value,
                                        SelectValueType select_value_type,
                                        std::vector<Declaration> & declarations) {
  switch (select_value_type) {
    case SelectValueType::SINGLE:
      qps_validator::ValidateSelectSynonym(select_value, declarations);
      break;
    case SelectValueType::MUTLIPLE:
      qps_validator::ValidateSelectTuple(select_value, declarations);
      break;
    default:
      throw QpsSyntaxError("Invalid select type");
  }
}

void qps_validator::ValidateStatementAfterResClause(std::string & remaining_statement) {
  if (!remaining_statement.empty()) {
    if (!QueryTokenizer::clauseMatch(remaining_statement, qps_constants::kPatternClauseRegex)
        && !QueryTokenizer::clauseMatch(remaining_statement, qps_constants::kSuchThatClauseRegex)) {
      throw QpsSyntaxError("Invalid syntax after Select synonym");
    }
  }
}

void qps_validator::ValidateAttributeRef(std::string & syn_string,
                                         std::string & attrName_string,
                                         std::vector<Declaration> declarations) {
  DesignEntity syn_design_entity;
  for (const Declaration& declaration : declarations) {
    if (syn_string == declaration.synonym) {
      syn_design_entity = declaration.design_entity;
      break;
    }
  }
  switch (syn_design_entity) {
    case DesignEntity::PROCEDURE:
      if (attrName_string != "procName") {
        throw QpsSemanticError("Procedure can only have .procName as attribute value");
      }
      break;
    case DesignEntity::STMT:
    case DesignEntity::WHILE_LOOP:
    case DesignEntity::IF_STMT:
    case DesignEntity::ASSIGN:
      if (attrName_string != "stmt#") {
        throw QpsSemanticError("Synonym can only have .stmt# as attribute value");
      }
      break;
    case DesignEntity::CALL:
      if (attrName_string != "procName" && attrName_string != "stmt#") {
        throw QpsSemanticError("Call can only have .stmt# or .procName as attribute value");
      }
      break;
    case DesignEntity::VARIABLE:
      if (attrName_string != "varName") {
        throw QpsSemanticError("Variable can only have .varName as attribute value");
      }
      break;
    case DesignEntity::READ:
    case DesignEntity::PRINT:
      if (attrName_string != "varName" && attrName_string != "stmt#") {
        throw QpsSemanticError("Synonym can only have .stmt# or .varName as attribute value");
      }
      break;
    case DesignEntity::CONSTANT:
      if (attrName_string != "value") {
        throw QpsSemanticError("Constant can only have .value as attribute value");
      }
      break;
    default:
      throw QpsSyntaxError("Unknown design entity synonym");
  }
}
