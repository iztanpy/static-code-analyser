#pragma once

#include <string>
#include <set>
#include <vector>
#include "qps/declaration.h"

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
}  // namespace qps_validator
