//
// Created by Isaac Tan on 12/9/23.
//

#include "ConstantStore.h"
typedef std::string constant;
typedef std::string possibleCombinations;
typedef int statementNumber;

ConstantStore::ConstantStore() {
    constants = std::unordered_set<constant>();
}

void ConstantStore::addConstants(std::unordered_set<constant> constants) {
    this->constants = constants;
}

std::unordered_set<constant> ConstantStore::getConstants() {
    return constants;
}


