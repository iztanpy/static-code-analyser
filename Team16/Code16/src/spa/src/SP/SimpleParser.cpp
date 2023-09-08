#include <stdio.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "SimpleParser.h"
//#include "TNode.h"
//#include "Visitor.h"
//#include "ParseVisitor.h"

//class ;

// constructor
SimpleParser::SimpleParser() {}
AssignmentParser::AssignmentParser() {}
bool isDebug = false;



int AssignmentParser::parse(const std::vector<Token>& tokens, int curr_index) {
    TNode lhs = TNode(tokens[curr_index].type, tokens[curr_index].name);
    TNode root = TNode(tokens[curr_index + 1].type, tokens[curr_index + 1].name);
    root.addChild(lhs);
    // check if rhs is expression
    curr_index += 2;

    while(curr_index <= tokens.size()) {
        Token curr = tokens[curr_index];
        if (isDebug) { std::cout << "curr token: " << curr.getValue() << std::endl; }

        // check if curr is constant or variable
        if (curr.type != "constant" && curr.type != "variable") {
            if (isDebug) { std::cout << "curr token is not a constant or variable" << std::endl; }
            return -1;
        }

        Token next = tokens[curr_index + 1];

        // check if next is ;
        if (next.type == ";") {
            if (isDebug) { std::cout << "next token is end token" << std::endl; }
            // add rhs to root
            root.addChild(TNode(curr.type, curr.getValue()));
            curr_index += 1;
            break;
        }

        // check if next is operator
        if (next.type != "plus" && next.type != "minus") {
            if (isDebug) { std::cout << "next token is not operator token" << std::endl; }
            return -1;
        }

        if (isDebug) { std::cout << "next token is operator" << std::endl; }

        // add operator as rhs of root
        TNode rhs = TNode(next.type, next.getValue());
        lhs = TNode(curr.type, curr.getValue());
        rhs.addChild(lhs);
        root.addChild(rhs);

        // update lhs and root
        root = rhs;

        // update curr_index
        curr_index += 2;
    }
    curr_index += 1;
    if (isDebug) { std::cout << "next index" << curr_index << std::endl; }
    return curr_index;
}

int SimpleParser::parse(std::vector<Token> tokens) {

    // Visitor
//    ParseVisitor* visitor = new ParseVisitor();
    AssignmentParser* assignmentParser = new AssignmentParser();

    //build ast
    int curr_index = 0;

    while(curr_index < tokens.size()) {
        Token curr_token = tokens[curr_index];
        if (curr_token.type == "variable") {
            Token next_token = tokens[curr_index + 1];
            if (next_token.type == "equal") {
                int next_index = assignmentParser->parse(tokens, curr_index);
                if (next_index == -1) {
                    if (isDebug) { std::cout << "Error: Invalid token in RHS of assignment" << std::endl; }
                    return -1;
                } else {
                    curr_index = next_index;
                }
            }
        }
        return curr_index;
    }

    return 0;
}

std::vector<std::string> SimpleParser::getAllVariables() {
    return std::vector<std::string>();
}