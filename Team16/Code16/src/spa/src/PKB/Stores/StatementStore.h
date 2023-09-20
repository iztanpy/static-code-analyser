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

    std::unordered_map<statementNumber, StmtEntity> statementMap;
    std::unordered_map<StmtEntity, std::unordered_set<statementNumber>> statementMapReverse;


 public:
    StatementStore();

    void addStatements(std::unordered_map<statementNumber, StmtEntity> typeMap);

    std::unordered_set<statementNumber> getStatements(StmtEntity type);

    std::unordered_set<statementNumber> getAllStatements();


};

