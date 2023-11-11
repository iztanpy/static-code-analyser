#include "VariableStore.h"
typedef std::string variable;
typedef std::string possibleCombinations;
typedef int statementNumber;

VariableStore::VariableStore() {
  variables = std::unordered_set<variable>();
}

void VariableStore::addVariables(std::unordered_set<variable> variables) {
  this->variables = variables;
}

std::unordered_set<variable> VariableStore::getVariables() {
  return variables;
}
