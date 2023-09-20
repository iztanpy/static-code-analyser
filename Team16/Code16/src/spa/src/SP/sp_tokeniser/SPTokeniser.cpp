/* Created by Shaine, */
// ai-gen start (gpt3, 2)
#include "SPTokeniser.h"

std::vector<std::pair<TokenType, std::regex>> regex_rules = {
    // operators
    {TokenType::kOperatorPlus, std::regex("^(\\+)")},
    {TokenType::kOperatorMinus, std::regex("^(-)")},
    {TokenType::kOperatorMultiply, std::regex("^(\\*)")},
    {TokenType::kOperatorDivide, std::regex("^(/)")},
    {TokenType::kOperatorEqual, std::regex("^(==)")},
    {TokenType::kOperatorNotEqual, std::regex("^(!=)")},
    {TokenType::kOperatorLessEqual, std::regex("^(<=)")},
    {TokenType::kOperatorLess, std::regex("^(<)")},
    {TokenType::kOperatorGreaterEqual, std::regex("^(>=)")},
    {TokenType::kOperatorGreater, std::regex("^(>)")},
    {TokenType::kOperatorLogicalAnd, std::regex("^(&&)")},
    {TokenType::kOperatorLogicalOr, std::regex("^(\\|\\|)")},
    {TokenType::kOperatorLogicalNot, std::regex("^(!)")},
    {TokenType::kOperatorMod, std::regex("^(%)")},


    // entity
    {TokenType::kEntityIf, std::regex("^(\\bif\\b)")},
    {TokenType::kEntityElse, std::regex("^(\\belse\\b)")},
    {TokenType::kEntityWhile, std::regex("^(\\bwhile\\b)")},
    {TokenType::kEntityRead, std::regex("^(\\bread\\b)")},
    {TokenType::kEntityProcedure, std::regex("^(\\bprocedure\\b)")},
    {TokenType::kEntityPrint, std::regex("^(\\bprint\\b)")},
    {TokenType::kEntityAssign, std::regex("^(=)")},
    {TokenType::kEntityCall, std::regex("^(\\bcall\\b)")},
    {TokenType::kEntityThen, std::regex("^(\\bthen\\b)")},

    // separators
    {TokenType::kSepComma, std::regex("^(,)")},
    {TokenType::kSepSemicolon, std::regex("^(;)")},
    {TokenType::kSepOpenParen, std::regex("^(\\()")},
    {TokenType::kSepCloseParen, std::regex("^(\\))")},
    {TokenType::kSepOpenBrace, std::regex("^(\\{)")},
    {TokenType::kSepCloseBrace, std::regex("^(\\})")},

    // Others
    {TokenType::kWhiteSpace, std::regex("^(\\s+)")},

    // literals
    {TokenType::kLiteralInteger, std::regex("^(\\b[+-]?\\d+\\b)")},
    {TokenType::kLiteralName, std::regex("^(\\b[0-9a-zA-Z]+\\b)")},


    //{ kLiteralLetter, std::regex("^([A-Za-z])")},
    //{ kLiteralDigit, std::regex("^([0-9])")},
};

/**
 * @brief Splits the input source program into separate lines and extracts tokens.
 *
 * This function takes an input string representing a source program and splits it into lines.
 * It then extracts tokens based on delimiters (';', '{', '}', '(', ')', ' ', '+', '-', '=', '*', '/', '%', '<', '>',
 * '&', '|', '!', '==', '!=', '<=', '>=', '||', '&&') and adds them to the result vector.
 *
 * @param input The input source program as a single string.
 * @return A vector of strings containing the extracted tokens from the source program.
 */
std::vector<std::string> SPtokeniser::splitLines(const std::string& input) {
    // Create an input string stream to process the input line by line
    std::istringstream iss(input);

    // Initialize a vector to store the extracted tokens
    std::vector<std::string> result;

    // Initialize a string to store each line
    std::string line;

    std::unordered_set<char> delimiters = {';', '{', '}', '+', '-', '=', '(', ')', '*', '/', '%', '<', '>', '&', '|',
                                           '!', ' '};

    // Iterate through each character in the line
    while (std::getline(iss, line)) {
        std::string currentToken;
        size_t start = 0;

        // filter out whitespaces
        size_t i = 0;
        while (i < line.length()) {
            // remove all white spaces
            std::string curr_word;
            // Iterate through each character in the line, filter out whitespaces
            size_t word_char_index = i;
            char curr_char = line[word_char_index];
            while (delimiters.find(curr_char) == delimiters.end()) {
                curr_word.push_back(curr_char);  // change to line.substr(start, i - start)
                curr_char = line[++word_char_index];
            }
            // add current word if not empty, accounts for double whitespaces and onwards
            if (!curr_word.empty()) result.push_back(curr_word);
            // include delimiters (excluding whitespaces) as tokens
            auto delimiterItr = delimiters.find(curr_char);
            if (delimiterItr != delimiters.end() &&  *delimiterItr != ' ') {
                curr_word = curr_char;
                // check if next char is a two-char operator
                if (word_char_index + 1 < line.length()) {
                    char next_char = line[word_char_index + 1];
                    if ((curr_word == "=" && next_char == '=')
                        || (curr_word == "!" && next_char == '=')
                        || (curr_word == "<" && next_char == '=')
                        || (curr_word == ">" && next_char == '=')
                        || (curr_word == "&" && next_char == '&')
                        || (curr_word == "|" && next_char == '|')) {
                        curr_word.push_back(next_char);
                        word_char_index++;
                    }
                }
                result.push_back(curr_word);
            }
            // only add non-whitespaces tokens, accounts for double whitespaces and onwards
            i = word_char_index + 1;
        }
    }

    return result;
}

/**
 * @brief Tokenizes the input source program into a vector of Token structures.
 *
 * This function takes an input source program as a string and tokenizes it, extracting tokens based on regular expressions.
 * It processes each line, identifies token types, and creates Token structures for each token found.
 * The resulting tokens are stored in a vector and returned.
 *
 * @param input The input source program as a single string.
 * @return A vector of Token structures representing the extracted tokens.
 * @throws std::runtime_error if an invalid token type or name is encountered.
 */
std::vector<struct Token> SPtokeniser::tokenise(const std::string& input) {
    std::vector<Token> tokens;
    std::vector<std::string> lines = splitLines(input);
    int lineNumber = 1;
    std::stack<char> braceStack;

    for (const std::string& line : lines) {
        int linePositionWithWhiteSpace = 0;
        int linePosition = 0;

        TokenType matchedType = TokenType::kUnknownTokenType;
        std::string matchedValue;
        for (const auto& regex_rule : regex_rules) {
            std::regex regex_pattern = regex_rule.second;
            std::smatch match;

            // Iterate through defined regular expressions to match token types
            if (std::regex_search(line.begin(), line.end(), match, regex_pattern)) {
                matchedType = regex_rule.first;
                matchedValue = match[0];
                break;
            }
        }

        if (matchedType == TokenType::kUnknownTokenType) { throw std::runtime_error("Invalid Token Type"); }

        if (matchedType == TokenType::kSepOpenBrace || matchedType == TokenType::kSepOpenParen) {
            braceStack.push(matchedValue[0]);
        }

        if (matchedType == TokenType::kWhiteSpace) {
            linePositionWithWhiteSpace++;
        } else if (matchedType == TokenType::kSepCloseBrace || matchedType == TokenType::kSepCloseParen) {
            if (braceStack.empty()) {
                throw std::runtime_error("Syntactic error: Unmatched closing brace");
            } else {
                char top = braceStack.top();
                if (matchedValue[0] == '}' && top == '{') {
                    braceStack.pop();
                    Token token{ matchedType, matchedValue, lineNumber, linePosition };
                    tokens.push_back(token);
                } else if (matchedValue[0] == ')' && top == '(') {
                    braceStack.pop();
                    // insert ')' as a token
                    Token token{matchedType, matchedValue, lineNumber, linePosition};
                    tokens.push_back(token);
                    linePosition += 1;
                    linePositionWithWhiteSpace += 1;
                } else {
                    throw std::runtime_error("Syntactic error: Unmatched closing parenthesis");
                }
            }
        } else if (matchedType == TokenType::kLiteralName) {
            if (std::isdigit(matchedValue[0])) {
                throw std::runtime_error("Invalid Token: Name cannot start with a digit");
            } else {
                Token token{matchedType, matchedValue, lineNumber, linePosition};
                tokens.push_back(token);
                linePositionWithWhiteSpace += matchedValue.length();
                linePosition += matchedValue.length();
            }
        } else {
            Token token{matchedType, matchedValue, lineNumber, linePosition};
            tokens.push_back(token);
            linePosition += 1;
            linePositionWithWhiteSpace += 1;
        }

        if (matchedType == TokenType::kSepSemicolon || matchedType == TokenType::kSepOpenBrace) { lineNumber++; }
    }
    return tokens;
}
