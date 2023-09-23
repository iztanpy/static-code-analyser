#include "ConditionParser.h"

/*
* ConditionParser returns -1 if the conditional parsing is not ok and returns 0 if the parsing is ok
*/
int ConditionParser::parse(std::vector<Token>& tokens, int curr_index) {
    // just need to verify if the condition is valid or not
    return evaluateCondition(tokens, 0) == -1 ? -1 : 0;
}


std::vector<Token> ConditionParser::getConditionTokens(const std::vector<Token>& tokens, int curr_index) {
    // Initialize condition tokens
    std::vector<Token> conditionTokens;

    // curr_index points to the "while" keyword
    if (curr_index + 1 < tokens.size() && tokens[curr_index + 1].tokenType == TokenType::kSepOpenParen) {
        int openParenCount = 1;
        conditionTokens.push_back(tokens[curr_index + 1]);
        int closeParenIndex = curr_index + 2;
        while (closeParenIndex < tokens.size() && openParenCount > 0) {
            if (tokens[closeParenIndex].tokenType == TokenType::kSepOpenParen) {
                openParenCount++;
            } else if (tokens[closeParenIndex].tokenType == TokenType::kSepCloseParen) {
                openParenCount--;
            }
            conditionTokens.push_back(tokens[closeParenIndex]);  // Add tokens within the condition
            closeParenIndex++;
        }

        if (openParenCount > 0) {
            // Implies that there is a missing closing parenthesis somewhere
//            throw InvalidSyntaxError();
          throw std::runtime_error("Syntactic error! We don't support anything and everything.");
        }
        curr_index = closeParenIndex;  // Modify curr_index to the new position
    } else {
        // Implies that there is a missing open parenthesis somewhere
//        throw InvalidSyntaxError();
      throw std::runtime_error("Syntactic error! We don't support anything and everything.");
    }
    return conditionTokens;
}

int ConditionParser::evaluateCondition(const std::vector<Token>& tokens, int curr_index) {
    return parseConditionExpression(tokens, curr_index);
}

int ConditionParser::parseConditionExpression(const std::vector<Token>& tokens, int curr_index) {
    // Parse logical expressions with operator precedence
    return parseLogicalExpression(tokens, curr_index);
}

int ConditionParser::parseLogicalExpression(const std::vector<Token>& tokens, int curr_index) {
    curr_index = parseNotExpression(tokens, curr_index);  // First, evaluate logical NOT

    bool valid = curr_index != -1;

    while (curr_index < tokens.size()) {
        const Token& token = tokens[curr_index];
        if (isLogicalOperator(token)) {
            curr_index++;  // Consume the logical operator
            if (curr_index < tokens.size()) {
                int rightOperandIndex = parseNotExpression(tokens, curr_index);  // Next, evaluate the right operand
                if (rightOperandIndex != -1) {
                    curr_index = rightOperandIndex;  // Update curr_index with the valid index
                    bool rightOperand = false;
                    if (token.tokenType == TokenType::kOperatorLogicalAnd) {
                        rightOperand = true;
                    } else if (token.tokenType == TokenType::kOperatorLogicalOr) {
                        rightOperand = true;
                    }
                    valid = valid && rightOperand;
                } else {
                    return -1;
                }
            } else {
                return -1;
            }
        } else {
            break;  // No more logical operators
        }
    }

    return valid ? curr_index : -1;
}


int ConditionParser::parseRelationalExpression(const std::vector<Token>& tokens, int curr_index) {
    if (curr_index >= tokens.size()) {
        return -1;  // Expression is empty
    }

    int updatedCurrIndex = parseRelFactor(tokens, curr_index);
    bool valid = updatedCurrIndex != -1;

    while (updatedCurrIndex < tokens.size() && isRelationalOperator(tokens[updatedCurrIndex])) {
        // Check for relational operators and continue parsing
        updatedCurrIndex++;
        if (updatedCurrIndex < tokens.size()) {
            updatedCurrIndex = parseRelFactor(tokens, updatedCurrIndex);
            valid = valid && (updatedCurrIndex != -1);
        } else {
            return -1;  // Invalid expression, operator at the end
        }
    }

    return valid ? updatedCurrIndex : -1;
}


int ConditionParser::parseRelFactor(const std::vector<Token>& tokens, int curr_index) {
    // Parse terms with operator precedence
    return parseTerm(tokens, curr_index);
}

int ConditionParser::parseTerm(const std::vector<Token>& tokens, int curr_index) {
    if (curr_index >= tokens.size()) {
        return -1;  // Expression is empty
    }

    int updatedCurrIndex = parseFactor(tokens, curr_index);

    while (updatedCurrIndex < tokens.size() &&
        (tokens[updatedCurrIndex].tokenType == TokenType::kOperatorMultiply ||
            tokens[updatedCurrIndex].tokenType == TokenType::kOperatorDivide)) {
        // Check for *, / operators and continue parsing
        updatedCurrIndex++;
        if (updatedCurrIndex < tokens.size()) {
            updatedCurrIndex = parseFactor(tokens, updatedCurrIndex);
        } else {
            return -1;
        }
    }

    return updatedCurrIndex;
}

int ConditionParser::parseFactor(const std::vector<Token>& tokens, int curr_index) {
    if (curr_index >= tokens.size()) {
        return -1;
    }

    const Token token = tokens[curr_index];

    if (token.tokenType == TokenType::kLiteralInteger ||
        token.tokenType == TokenType::kLiteralDigit ||
        token.tokenType == TokenType::kLiteralName) {
        curr_index++;
        return curr_index;  // Return the updated curr_index
    } else if (token.tokenType == TokenType::kSepOpenParen) {
        // Parse a sub-expression within parentheses
        curr_index++;
        int updatedCurrIndex = parseConditionExpression(tokens, curr_index);

        if (updatedCurrIndex < tokens.size() && tokens[updatedCurrIndex].tokenType == TokenType::kSepCloseParen) {
            return updatedCurrIndex + 1;  // Return the updated curr_index after consuming ')'
        } else {
            return -1;
        }
    } else {
        return -1;  // Invalid token
    }
}

int ConditionParser::parseNotExpression(const std::vector<Token>& tokens, int curr_index) {
    if (curr_index < tokens.size() && tokens[curr_index].tokenType == TokenType::kOperatorLogicalNot) {
        curr_index++;  // Consume the logical NOT operator
        int operandIndex = parseRelationalExpression(tokens, curr_index);  // Evaluate the operand
        if (operandIndex != -1) {
            return operandIndex;
        } else {
            return -1;
        }
    } else {
        return parseRelationalExpression(tokens, curr_index);  // No logical NOT, evaluate the operand directly
    }
}


// UTILS
bool ConditionParser::isLogicalOperator(const Token& token) {
    return token.tokenType == TokenType::kOperatorLogicalAnd ||
        token.tokenType == TokenType::kOperatorLogicalNot ||
        token.tokenType == TokenType::kOperatorLogicalOr;
}

bool ConditionParser::isRelationalOperator(const Token& token) {
    return token.tokenType == TokenType::kOperatorNotEqual ||
        token.tokenType == TokenType::kOperatorGreater ||
        token.tokenType == TokenType::kOperatorLess ||
        token.tokenType == TokenType::kOperatorLessEqual ||
        token.tokenType == TokenType::kOperatorGreaterEqual ||
        token.tokenType == TokenType::kOperatorEqual;
}
