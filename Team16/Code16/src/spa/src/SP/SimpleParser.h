#pragma once
#ifndef SPA_SIMPLEPARSER_H
#define SPA_SIMPLEPARSER_H

#include <vector>
#include <string>
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
