#pragma once

#include <string>
#include <vector>

namespace string_util {
/*!
 * Trim white spaces from the beginning of a string
 * @param str the string to trim
 * @return string trimmed at the beginning
 */
std::string LeftTrim(const std::string & str);

/*!
 * Trim white spaces from the end of a string
 * @param str the string to trim
 * @return string trimmed at the end
 */
std::string RightTrim(const std::string & str);

/*!
 * Trim white spaces from the beginning and end of a string
 * @param str the string to trim
 * @return trimmed string
 */
std::string Trim(const std::string & str);

/*!
 * Trims the extra white spaces in within the string
 * @param str the string to trim
 * @return trimmed string
 */
std::string RemoveWhiteSpaces(const std::string & str);

/*!
 * Checks if the string is a white space
 * @param str is the specified string
 * @return true if the string is a white space, else false
 */
bool IsWhiteSpace(const std::string & str);

/*!
 * Splits string into a vector of strings wrt to a delimiter
 * @param delimiter where the string should split
 * @param str the string to split
 * @return a vector of strings split according to delimiter
 */
std::vector<std::string> SplitStringBy(const char & delimiter, std::string & str);

/*!
 * Returns the first word of specified string
 * @param str specified string
 * @return first word as string
 */
std::string GetFirstWord(std::string & str);

/*!
 * Return the first word from an argument e.g. Uses(a, v)
 * @param str specified string
 * @return first word of the argument e.g. Uses
 */
std::string GetFirstWordFromArgs(std::string & str);

/*!
 * Removes the first word from an argument e.g. Uses(a, v)
 * @param str specified string
 * @return the remaining string e.g., (a, v)
 */
std::string RemoveFirstWordFromArgs(std::string & str);

/*!
 * Removes the first word of the specified string
 * @param str specified string
 * @return the remainder of the string
 */
std::string RemoveFirstWord(std::string & str);

/*!
 * Removes the white spaces from an expression
 * @param str the expression to be parsed
 * @return expression with white spaces removed
 */
std::string RemoveSpacesFromExpr(const std::string & str);
}  // namespace string_util
