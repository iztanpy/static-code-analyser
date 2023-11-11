#include "PKB/Stores/constant_store.h"
typedef std::string constant;
typedef std::string possibleCombinations;
typedef int statementNumber;

constant_store::constant_store() {
  constants = std::unordered_set<constant>();
}

void constant_store::addConstants(std::unordered_set<constant> constants) {
  this->constants = constants;
}

std::unordered_set<constant> constant_store::getConstants() {
  return constants;
}


