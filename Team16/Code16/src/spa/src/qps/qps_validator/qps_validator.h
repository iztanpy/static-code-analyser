#pragma once

#include <string>
#include <set>
#include <vector>
#include "qps/declaration.h"
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"

namespace qps_validator {
/*!
 * Validates the syntax of split queries
 * @param statement to be validated
 * @param is_select_statement_processed boolean indicator
 * @throws QpsSyntaxError exception if syntax of statement is invalid
 */
void ValidateStatement(std::string statement, bool is_select_statement_processed);

/*!
 * Validates the existence of a select statement
 * @param select_statement to be validated
 * @throws QpsSyntaxError exception if select statement is missing
 */
void ValidateSelectExists(const std::string& select_statement);

/*!
 * Validates the syntax of a declaration
 * @param statement to be validated
 * @throws QpsSyntaxError exception if syntax of declaration is invalid
 */
void ValidateDeclarationStatement(std::string statement);

/*!
 * Validates the syntax of a synonym in a declaration
 * @param synonym to be validated
 * @param processed_synonyms are synonyms that have been declared before
 * @throws QpsSyntaxError exception if syntax of synonym is invalid
 * @throws QpsSemanticError exception if synonym is repeated
 */
void ValidateDeclarationSynonym(std::string synonym, std::set<std::string> & processed_synonyms);

/*!
 * Validates the select clause statement
 * @param select_statement to be validated
 * @throws QpsSyntaxErrorException if arguments are missing
 */
void ValidateSelectStatement(std::string select_statement);

/*!
 * Validates select synonym
 * @param select_synonym to be validated
 * @param declarations are synonyms that have been declared
 * @throws QpsSyntaxError if synonym syntax is invalid
 * @throws QpsSemanticError if synonym has not been declared
 */
void ValidateSelectSynonym(std::string select_synonym, std::vector<Declaration> & declarations);

/*!
 * Validates clause indexes
 * @param indexes
 * @throws QpsSyntaxError if clauses are syntatically incorrect or clause does not start immediately
 * after Select clause e.g., Select v ____ such that _____
 */
void ValidateClauseIndexes(std::vector<size_t>& indexes);

/*!
 * Validates the clause arguments
 * @param clause to be validated
 * @throws QpsSyntaxError if clause syntax is invalid
 */
void ValidateClauseArgs(std::string clause);

/*!
 * Validates the pattern clause arguments
 * @param clause to be validated
 * @param pattern_type
 * @throws QpsSyntaxError if clause syntax is invalid
 */
void ValidateClauseArgs(std::string clause, PQLTokenType pattern_type);

/*!
 * Validates the individual arguments of clause
 * @param lhs left argument to be validated
 * @param rhs right argument to be validated
 * @throws QpsSyntaxError if either arguments have invalid syntax
 */
void ValidateClauseArgs(std::string lhs, std::string rhs);

/*!
 * Validates the clause synonyms
 * @param synonym to be validated
 * @param declarations set of declarations
 * @throws QpsSemanticError if the synonym has not been declared
 */
void ValidateClauseSynonym(std::string synonym, std::vector<Declaration> & declarations);

/*!
 * Validates the pattern synonyms
 * @param pattern_synonym to be validated
 * @param declarations set of declarations
 * @throws QpsSemanticError if the pattern synonym is not an assignment, if or while statement
 */
void ValidatePatternSynonym(std::string pattern_synonym, std::vector<Declaration> & declarations);

/*!
 * Additional check that third argument if a wildcard
 * @param third_token token to be checked
 */
void ValidateIfPatternClause(std::string third_token);

/*!
 * Validates the individual arguments of pattern clause
 * @param left_hand_side
 * @param right_hand_side
 * @param pattern_type
 * @throws QpsSyntaxError if either arguments have invalid syntax
 */
void ValidatePatternClauseArgs(const std::string& left_hand_side,
                               const std::string& right_hand_side,
                               PQLTokenType pattern_type);

/*!
 * Validates the syntax of such that clause
 * @param clause_with_keyword_removed
 * @throws QpsSyntaxError if there is missing input
 */
void ValidateNonEmptyClause(const std::string & clause_with_keyword_removed);

/*!
 * Validates if string is a relation reference
 * @param rel_ref
 * @throws QpsSyntaxError if it is not a relation reference
 */
void ValidateRelRef(const std::string & rel_ref);
}  // namespace qps_validator
