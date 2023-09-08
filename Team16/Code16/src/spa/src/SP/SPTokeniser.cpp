/* Created by Shaine, */
// ai-gen start (gpt3, 2)
#include "SPTokeniser.h"


std::vector<std::pair<TokenType, std::regex>> regex_rules = {
    // operators 
    { TokenType::kOperatorPlus, std::regex("^(\\+)") },
    { TokenType::kOperatorMinus, std::regex("^(-)") },
    { TokenType::kOperatorMultiply, std::regex("^(\\*)") },
    { TokenType::kOperatorDivide, std::regex("^(/)") },
    { TokenType::kOperatorEqual, std::regex("^(==)") },
    { TokenType::kOperatorNotEqual, std::regex("^(!=)") },
    { TokenType::kOperatorLessEqual, std::regex("^(<=)") },
    { TokenType::kOperatorLess, std::regex("^(<)") },
    { TokenType::kOperatorGreaterEqual, std::regex("^(>=)") },
    { TokenType::kOperatorGreater, std::regex("^(>)") },
    { TokenType::kOperatorLogicalAnd, std::regex("^(&&)") },
    { TokenType::kOperatorLogicalOr, std::regex("^(\\|\\|)") },
    { TokenType::kOperatorLogicalNot, std::regex("^(!)") },
    { TokenType::kOperatorMod, std::regex("^(%)") },


    // entity 
    { TokenType::kEntityIf, std::regex("^(\\bif\\b)") },
    { TokenType::kEntityElse, std::regex("^(\\belse\\b)") },
    { TokenType::kEntityWhile, std::regex("^(\\bwhile\\b)") },
    { TokenType::kEntityRead, std::regex("^(\\bread\\b)")},
    { TokenType::kEntityProcedure, std::regex("^(\\bprocedure\\b)") },
    { TokenType::kEntityPrint, std::regex("^(\\bprint\\b)") },
    { TokenType::kEntityAssign, std::regex("^(=)") },
    { TokenType::kEntityCall, std::regex("^(\\bcall\\b)") },

 
    // separators 
    { TokenType::kSepComma, std::regex("^(,)")},
    { TokenType::kSepSemicolon, std::regex("^(;)") },
    { TokenType::kSepOpenParen, std::regex("^(\\()") },
    { TokenType::kSepCloseParen, std::regex("^(\\))") },
    { TokenType::kSepOpenBrace, std::regex("^(\\{)") },
    { TokenType::kSepCloseBrace, std::regex("^(\\})") },

    // Others 
    {  TokenType::kWhiteSpace,  std::regex("^(\\s+)") },

    // literals 
    { TokenType::kLiteralInteger, std::regex("^(\\b[+-]?\\d+\\b)") },
    { TokenType::kLiteralName, std::regex("^(\\b[0-9a-zA-Z]+\\b)") },


    //{ kLiteralLetter, std::regex("^([A-Za-z])")},
    //{ kLiteralDigit, std::regex("^([0-9])")}, 
};



// Performs the delineation of the source program
std::vector<std::string> SPtokeniser::splitLines(const std::string& input)
{
    std::istringstream iss(input);
    std::vector<std::string> result;
    std::string line;

    while (std::getline(iss, line)) {
        std::string currentToken;
        size_t start = 0;

        for (size_t i = 0; i < line.length(); ++i) {
            if (line[i] == ';' || line[i] == '{' || line[i] == '}') {
                // Add the token before the delimiter or bracket
                if (i > start) {
                    currentToken = line.substr(start, i - start);
                    result.push_back(currentToken);
                }

                // Add the delimiter or bracket itself as a token
           /*     currentToken = line[i];
                result.push_back(currentToken);*/

                start = i + 1; // Move the start position after the delimiter or bracket
            }
        }

        // Add the remaining portion of the line as a token
        if (start < line.length()) {
            currentToken = line.substr(start);
            result.push_back(currentToken);
        }
    }


    return result;
}



std::vector<struct Token> SPtokeniser::tokenise(const std::string& input) {
    std::vector<Token> tokens;
    std::vector<std::string> lines = splitLines(input);
    int lineNumber = 1;

    for (const std::string& line : lines) {
        int linePosition = 0;

        while (linePosition < line.length()) {
            TokenType matchedType = TokenType::kUnknownTokenType;
            std::string matchedValue;

            for (const auto& regex_rule : regex_rules) {
                std::regex regex_pattern = regex_rule.second;
                std::smatch match;

                if (std::regex_search(line.begin() + linePosition, line.end(), match, regex_pattern)) {
                    matchedType = regex_rule.first;
                    matchedValue = match[0];
                    break;
                }
            }

            if (matchedType == TokenType::kUnknownTokenType) { throw std::runtime_error("Invalid Token Type"); }

            if (matchedType == TokenType::kWhiteSpace && linePosition + matchedValue.length() < line.length()) {
                linePosition++;
            }
            else if (matchedType == TokenType::kLiteralName) {
                if (std::isdigit(matchedValue[0])) {
                    throw std::runtime_error("Invalid Token: Name cannot start with a digit");
                }
                else {
                    Token token{ matchedType, matchedValue, lineNumber, linePosition };
                    tokens.push_back(token);
                    linePosition += matchedValue.length();
                }
            } else {
                Token token{ matchedType, matchedValue, lineNumber, linePosition };
                tokens.push_back(token);
                linePosition += matchedValue.length();
            }
        }

        lineNumber++; 
    } 

    return tokens;
}
