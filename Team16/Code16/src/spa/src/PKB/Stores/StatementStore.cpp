//
// Created by Isaac Tan on 13/9/23.
//

#include "StatementStore.h"

typedef std::string variable;
typedef std::string constant;
typedef int statementNumber;
typedef std::string statementType;

StatementStore::StatementStore() {
    this->numTypeMap = std::unordered_map<statementNumber, statementType>();
}

void StatementStore::addStatement(std::unordered_map<statementNumber, statementType> typeMap) {
    this->numTypeMap = typeMap;
}

std::unordered_set<statementNumber> StatementStore::getStatements(StatementStore::statementType type) {
    std::unordered_set<statementNumber> statements;
    for (auto const& x : this->numTypeMap) {
        if (x.second == type) {
            statements.insert(x.first);
        }
    }
    return statements;
}
