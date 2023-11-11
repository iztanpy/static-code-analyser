#include "PKB/Stores/statement_store.h"
#include <unordered_map>

typedef std::string variable;
typedef std::string constant;
typedef int statementNumber;

statement_store::statement_store() {
  this->statementMap = std::unordered_map<statementNumber, StmtEntity>();
}

void statement_store::addStatements(std::unordered_map<statementNumber, StmtEntity> typeMap) {
  this->statementMap = typeMap;
  for (auto const& x : typeMap) {
    this->statementMapReverse[x.second].insert(x.first);
  }
}

std::unordered_set<statementNumber> statement_store::getStatements(StmtEntity type) {
  if (type == StmtEntity::kStmt) {
    return this->getAllStatements();
  }
  std::unordered_set<statementNumber> statements;
  for (auto const& x : this->statementMap) {
    if (x.second == type) {
      statements.insert(x.first);
    }
  }
  return statements;
}

std::unordered_set<statementNumber> statement_store::getAllStatements() {
  std::unordered_set<statementNumber> statements;
  for (auto const& x : this->statementMap) {
    statements.insert(x.first);
  }
  return statements;
}

bool statement_store::isPresent(statementNumber statement) {
  return this->statementMap.find(statement) != this->statementMap.end();
}

bool statement_store::isAssign(statementNumber statement) {
  return this->statementMap[statement] == StmtEntity::kAssign;
}
