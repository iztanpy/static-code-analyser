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

#include "TNode.h"
#include "Token.h"
#include "DesignExtractor.h"

class Parser {
public:
    Parser() = default;
    virtual ~Parser() = default;
    virtual int parse(const std::vector<Token>& tokens) = 0;
    int curr_index = 0;
    DesignExtractor* designExtractor = new DesignExtractor();
};

class AssignmentParser : public Parser {
public:
    AssignmentParser() = default;
    int parse(const std::vector<Token>& tokens) override;
    ASTVisitor* visitor = new ASTVisitor();

    unordered_map<string, unordered_set<string>> getAssignVarHashmap();
    unordered_map<string, unordered_set<string>> getAssignConstHashmap();
    unordered_set<string> getVariablesHashset();
    unordered_set<string> getConstantsHashset();
};

class SimpleParser : public Parser {
    public:
        SimpleParser() = default;
        int parse(const std::vector<Token>& tokens) override;
        AssignmentParser* assignmentParser = new AssignmentParser();

        unordered_map<string, unordered_set<string>> getAssignVarHashmap();
        unordered_map<string, unordered_set<string>> getAssignConstHashmap();
        unordered_set<string> getVariablesHashset();
        unordered_set<string> getConstantsHashset();
};

#endif //SPA_SIMPLEPARSER_H
