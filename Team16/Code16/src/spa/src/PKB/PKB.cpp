#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "PKB.h"

PKB::PKB(std::vector<int> assignments, std::vector<std::string> variables, std::vector<std::string> constants) {
    setPKB(assignments, variables, constants);
}

PKB::PKB() {
}

void PKB::setPKB(std::vector<int> assignments, std::vector<std::string> variables, std::vector<std::string> constants) {
    this->assignments = assignments;
    this->variables = variables;
    this->constants = constants;
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

std::vector<int> PKB::getAssignments() {
    return this->assignments;
}

std::vector<std::string> PKB::getVariables() {
    return this->variables;
}

std::vector<std::string> PKB::getConstants() {
    return this->constants;
}
