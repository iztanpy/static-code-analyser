#include "PKB/PKB.h"

PKB::PKB(std::unordered_set<int> assignments,
         std::unordered_set<std::string> variables,
         std::unordered_set<std::string> constants,
         std::unordered_map<std::string, std::unordered_set<std::string>> UsesConst,
         std::unordered_map<std::string, std::unordered_set<std::string>> UsesVar,
         std::unordered_map<int, std::unordered_set<std::string>> LineUses) {
  setPKB(assignments, variables, constants, UsesConst, UsesVar, LineUses);
}

PKB::PKB() {
}

void PKB::setPKB(std::unordered_set<int> assignments,
                 std::unordered_set<std::string> variables,
                 std::unordered_set<std::string> constants,
                 std::unordered_map<std::string, std::unordered_set<std::string>> UsesConst,
                 std::unordered_map<std::string, std::unordered_set<std::string>> UsesVar,
                 std::unordered_map<int, std::unordered_set<std::string>> LineUses) {
  this->assignments = assignments;
  this->variables = variables;
  this->constants = constants;
  this->UsesConst = UsesConst;
  this->UsesVar = UsesVar;
  this->LineUses = LineUses;
}

void PKB::setAssignments(std::unordered_set<int> assignments) {
  this->assignments = assignments;
}
void PKB::setVariables(std::unordered_set<std::string> variables) {
  this->variables = variables;
}
void PKB::setConstants(std::unordered_set<std::string> constants) {
  this->constants = constants;
}

void PKB::setUsesConst(std::unordered_map<std::string, std::unordered_set<std::string>> UsesConst) {
  this->UsesConst = UsesConst;
}

void PKB::setUsesVar(std::unordered_map<std::string, std::unordered_set<std::string>> UsesVar) {
  this->UsesVar = UsesVar;
}

void PKB::setLineUses(std::unordered_map<int, std::unordered_set<std::string>> LineUses) {
  this->LineUses = LineUses;
}

std::unordered_set<int> PKB::getAssignments() {
  return this->assignments;
}

std::unordered_set<std::string> PKB::getVariables() {
  return this->variables;
}

std::unordered_set<std::string> PKB::getConstants() {
  return this->constants;
}

std::unordered_set<std::string> PKB::getVariablesUsedBy(int lineNumber) {
  return this->LineUses[lineNumber];
}
