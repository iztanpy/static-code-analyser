#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "../Helper/StmtEntity.h"


class StatementStore {
 private:
    typedef std::string variable;
    typedef std::string constant;
    typedef int statementNumber;
    typedef std::string statementType;

    std::unordered_map<statementNumber, StmtEntity> numTypeMap;


 public:
    StatementStore();

    void addStatements(std::unordered_map<statementNumber, StmtEntity> typeMap);

    std::unordered_set<statementNumber> getStatements(StmtEntity type);
};

