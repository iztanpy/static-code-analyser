#pragma once
#ifndef SPA_SIMPLEPARSER_H
#define SPA_SIMPLEPARSER_H

#include <vector>
#include <string>
#include "../TNode.h"
#include "ParseVisitor.h"

struct Token {
    std::string type;
    int line;

    std::string name;
    std::string intValue;

    Token() : type(), line(), name(), intValue() {};
    Token(std::string t) : type(t), line(), name(), intValue() {};
    Token(std::string t, int l) : type(t), line(l), name(), intValue() {};
    Token(std::string t, int l, int i) : type(t), line(l), name(), intValue(std::to_string(i)) {};
    Token(std::string t, int l, std::string n) : type(t), line(l), name(n), intValue() {};

    std::string getValue() {
        if (type == "variable") {
            return name;
        } else if (type == "constant") {
            return intValue;
        } else {
            return type;
        }
    }
};

class SimpleParser {
    public:
        SimpleParser();
//        ParseVisitor* visitor;
        int parse(std::vector<Token> tokens);
        std::vector<std::string> getAllVariables();

};

class AssignmentParser {
public:
    AssignmentParser();
    int parse(const std::vector<Token>& tokens, int curr_index);
};

class Expression {
public:
    Expression() = default;
    static bool isExpression() {
        return true;
    };
};
#endif //SPA_SIMPLEPARSER_H
