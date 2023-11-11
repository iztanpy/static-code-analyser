#include "PKB/Stores/variable_store.h"
typedef std::string variable;
typedef std::string possibleCombinations;
typedef int statementNumber;

variable_store::variable_store() {
  variables = std::unordered_set<variable>();
}

void variable_store::addVariables(std::unordered_set<variable> variables) {
  this->variables = variables;
}

std::unordered_set<variable> variable_store::getVariables() {
  return variables;
}
