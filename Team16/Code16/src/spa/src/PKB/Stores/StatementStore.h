//
// Created by Isaac Tan on 13/9/23.
//

#define SPA_STATEMENTSTORE_H

#include <string>
#include <unordered_set>


class StatementStore {
private:
    typedef std::string variable;
    typedef std::string constant;
    typedef int statementNumber;
    typedef std::string statementType;

    std::unordered_map<statementNumber, statementType> numTypeMap;


public:
    StatementStore();

    void addStatement(std::unordered_map<statementNumber, statementType> typeMap);

    std::unordered_set<statementNumber> getStatements(statementType type);


};

