#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "qps/declaration.h"

class QueryUtil {
 public:
  /*!
   * Checks if a given string is a wildcard
   * @param s is the input string
   * @return True if string is a wildcard, else false
   */
  static bool IsWildcard(const std::string & s);

  /*!
   * Checks if a given string is a synonym
   * @param s is the input string
   * @return True if string is a synonym, else false
   */
  static bool IsSynonym(const std::string & s);

  /*!
   * Checks if a given string is a statement reference
   * @param s is the input string
   * @return True if string is a statement reference, else false
   */
  static bool IsStmtRef(const std::string & s);

  /*!
   * Checks if a given string is an entity reference
   * @param s is the input string
   * @return True if string is a entity reference, else false
   */
  static bool IsEntRef(const std::string & s);

  /*!
   * Checks if a string belongs to any design entity
   * @param s is the input string
   * @return True if string is a design entity, else false
   */
  static bool IsDesignEntity(const std::string & s);

  /*!
   * Checks if a string is ina set of declarations
   * @param s is the input string
   * @param declarations the set of declared entities
   * @return True if the string is in a set of declarations, else false
   */
  static bool IsInDeclarations(const std::string & s, const std::vector<Declaration> & declarations);

  /*!
   * Checks if a string is a synonym and an assignment
   * @param s is the input string
   * @param declarations is the vector of declarations
   * @return True if string is a synonym and declared as an assignment, else false
   */
  static bool IsSynAssign(const std::string & s, const std::vector<Declaration> & declarations);

  /*!
   * Checks if a string is a synonym and an if statement
   * @param s is the input string
   * @param declarations is the vector of declarations
   * @return True if string is a synonym and declared as an if statement, else false
   */
  static bool IsSynIf(const std::string & s, const std::vector<Declaration> & declarations);

  /*!
   * Checks if a string is a synonym and a while statement
   * @param s is the input string
   * @param declarations is the vector of declarations
   * @return True if string is a synonym and declared as a while statement, else false
   */
  static bool IsSynWhile(const std::string & s, const std::vector<Declaration> & declarations);

  /*!
   * Checks if a string quoted
   * @param s is the input string
   * @return True if string is quoted, else false
   */
  static bool IsEnclosedInDoubleQuotes(const std::string & s);

  /*!
   * Checks if a string is IDENT with quotes
   * @param s is the input string
   * @return True if string is IDENT with quotes, else false
   */
  static bool IsIdentWithDoubleQuotes(const std::string & s);

  /*!
 * Checks if a string is enclosed with wildcards
 * @param s is the input string
 * @return True if string is enclosed with wildcard, else false
 */
  static bool IsEnclosedInWildcard(const std::string & s);

  /*!
* Checks if a string is enclosed with brackets
* @param s is the input string
* @return True if string is enclosed with brackets, else false
*/
  static bool IsEnclosedInBrackets(const std::string & s);

  /*!
   * Checks if a string is a relationship reference
   * @param s is the input string
   * @return True if string is a relationship reference, else false
   */
  static bool IsRelRef(const std::string & s);

  /*!
   * Checks if a string is an expression specification
   * @param s is the input string
   * @return True if the string is an expression specification, else false
   */
  static bool IsExprSpec(const std::string & s);

  /*!
   * Checks if a string is an an exact expression specification
   * @param s is the input
   * @return True if the string is an exact expression, else false
   */
  static bool IsExactExpressionSpecification(const std::string & s);

  /*!
   * Checks if an expression is partial match
   * @param s is the input string
   * @return True if string is surrounded by '_', else false
   */
  static bool IsPartialMatchExpressionSpecification(const std::string & s);

  /*!
   * Get the IDENT inside quotations.
   * @param quoted_ident Quoted IDENT
   * @return The ident with quotations removed.
   */
  static std::string RemoveQuotations(const std::string & quoted_ident);

  /*!
   * Get the expression between the wildcards
   * @param partial_match_string
   * @return The expression
   */
  static std::string RemovePartialMatch(const std::string & partial_match_string);

  /*!
 * Get the expression between the brackets
 * @param s
 * @return The expression between the brackets
 */
  static std::string RemoveBrackets(const std::string & s);
};
