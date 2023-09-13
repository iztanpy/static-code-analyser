//
// Created by Isaac Tan on 12/9/23.
//

#ifndef SPA_USESSTORE_H
#define SPA_USESSTORE_H

#include <string>
#include <unordered_set>


class UsesStore {
private:
    typedef std::string variable;
    typedef std::string constant;
    typedef int statementNumber;

    std::unordered_map<statementNumber, std::unordered_set<variable>> UsesVariableMap;
    std::unordered_map<statementNumber, std::unordered_set<constant>> UsesConstantMap;

public:
    UsesStore();

    void addLineUsesVar(std::unordered_map<statementNumber, std::unordered_set<variable>> statementUsesMap);

    void addLineUsesConst(std::unordered_map<statementNumber, std::unordered_set<constant>> variableUsesMap);

    std::unordered_set<variable> getVariablesUsedBy(statementNumber lineNumber);

    std::unordered_set<constant> getConstantsUsedBy(statementNumber lineNumber);


};


#endif //SPA_USESSTORE_H
