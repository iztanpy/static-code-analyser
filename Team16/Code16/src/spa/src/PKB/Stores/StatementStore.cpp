#include "StatementStore.h"
#include <unordered_map>'

typedef std::string variable;
typedef std::string constant;
typedef int statementNumber;

StatementStore::StatementStore() {
    this->numTypeMap = std::unordered_map<statementNumber, StmtEntity>();
}

void StatementStore::addStatement(std::unordered_map<statementNumber, StmtEntity> typeMap) {
    this->numTypeMap = typeMap;
}

std::unordered_set<statementNumber> StatementStore::getStatements(StmtEntity type) {
    std::unordered_set<statementNumber> statements;
    for (auto const& x : this->numTypeMap) {
        if (x.second == type) {
            statements.insert(x.first);
        }
    }
    return statements;
}
