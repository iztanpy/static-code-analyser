#pragma once
#ifndef SPA_SIMPLEPARSER_H
#define SPA_SIMPLEPARSER_H

#include <vector>
#include <string>
#include "TNode.h"
#include "Token.h"
#include "DesignExtractor.h"
#include <unordered_map>
#include <unordered_set>
#include <memory>

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

    std::unordered_map<std::string, std::unordered_set<std::string>> getAssignVarHashmap();
    std::unordered_map<std::string, std::unordered_set<std::string>> getAssignConstHashmap();
    std::unordered_set<std::string> getVariablesHashset();
    std::unordered_set<std::string> getConstantsHashset();
};

class SimpleParser : public Parser {
 public:
        SimpleParser() = default;
        int parse(const std::vector<Token>& tokens) override;
        AssignmentParser* assignmentParser = new AssignmentParser();

        std::unordered_map<std::string, std::unordered_set<std::string>> getAssignVarHashmap();
        std::unordered_map<std::string, std::unordered_set<std::string>> getAssignConstHashmap();
        std::unordered_set<std::string> getVariablesHashset();
        std::unordered_set<std::string> getConstantsHashset();
};

#endif  //  SPA_SIMPLEPARSER_H
