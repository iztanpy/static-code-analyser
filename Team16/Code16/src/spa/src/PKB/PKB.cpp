#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

#include "PKB.h"


PKB::PKB(std::vector<int> assignments,
         std::vector<std::string> variables,
         std::vector<std::string> constants,
         std::unordered_map<std::string, std::unordered_set<std::string>>  UsesConst,
         std::unordered_map<std::string, std::unordered_set<std::string>>  UsesVar,
         std::unordered_map<int, std::unordered_set<std::string>> LineUses) {
    setPKB(assignments, variables, constants, UsesConst, UsesVar, LineUses);
}

PKB::PKB() {
}

void PKB::setPKB(std::vector<int> assignments,
                 std::vector<std::string> variables,
                 std::vector<std::string> constants,
                 std::unordered_map<std::string, std::unordered_set<std::string>>  UsesConst,
                 std::unordered_map<std::string, std::unordered_set<std::string>>  UsesVar,
                 std::unordered_map<int, std::unordered_set<std::string>> LineUses) {
    this->assignments = assignments;
    this->variables = variables;
    this->constants = constants;
    this->UsesConst = UsesConst;
    this->UsesVar = UsesVar;
    this->LineUses = LineUses;
}

void PKB::setAssignments(std::vector<int> assignments) {
    this->assignments = assignments;
}
void PKB::setVariables(std::vector<std::string> variables) {
    this->variables = variables;
}
void PKB::setConstants(std::vector<std::string> constants) {
    this->constants = constants;
}

void PKB::setUsesConst(std::unordered_map<std::string, std::unordered_set<std::string>>  UsesConst) {
    this->UsesConst = UsesConst;
}

void PKB::setUsesVar(std::unordered_map<std::string, std::unordered_set<std::string>>  UsesVar) {
    this->UsesVar = UsesVar;
}

void PKB::setLineUses(std::unordered_map<int, std::unordered_set<std::string>> LineUses ) {
    this->LineUses = LineUses;
}

std::vector<int> PKB::getAssignments() {
    return this->assignments;
}

std::vector<std::string> PKB::getVariables() {
    return this->variables;
}

std::vector<std::string> PKB::getConstants() {
    return this->constants;
}

std::unordered_set<std::string> PKB::getVariablesUsedBy(int lineNumber) {
    return this->LineUses[lineNumber];
}
