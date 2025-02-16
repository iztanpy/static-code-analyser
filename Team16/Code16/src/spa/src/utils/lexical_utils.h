#pragma once

#include <string>

namespace lexical_utils {
/*!
 * Checks if a character is a letter
 * @param c is the specified character
 * @return true if the character is a letter, else false
 */
bool IsLetter(char c);

/*!
 * Checks if a character is a digit
 * @param c is the specified character
 * @return true if the character is a digit, else false
 */
bool IsDigit(char c);

/*!
 * Checks if a character is a NzDigit
 * @param c is the specified character
 * @return true if the character is a NzDigit, else false
 */
bool IsNzDigit(char c);

/*!
 * Checks if a character is a IDENT
 * @param c is the specified character
 * @return true if the character is a IDENT, else false
 */
bool IsIdent(std::string str);

/*!
 * Checks if a character is a Synonym
 * @param c is the specified character
 * @return true if the character is a Synonym, else false
 */
bool IsSynonym(std::string str);

// bool IsExpr(std::string str);
// bool IsTerm(std::string str);
// bool IsFactor(std::string str);

/*!
 * Checks if a character is a name
 * @param c is the specified character
 * @return true if the character is a name, else false
 */
bool IsName(std::string str);

/*!
 * Checks if a character is an integer
 * @param c is the specified character
 * @return true if the character is an integer, else false
 */
bool IsInteger(std::string str);

/*!
 * Checks if a string is an expression
 * @param str
 * @return
 */
bool IsExpr(std::string str);

/*!
 * Checks if a string is a Factor
 * @param str is the string to be checked
 * @return True if the string is a Factor, else false
 */
bool IsFactor(std::string str);

/*!
 * Checks if a string is a Term
 * @param str is the string to be checked
 * @return True if the string is a term, else false
 */
bool IsTerm(std::string str);
}  // namespace lexical_utils
