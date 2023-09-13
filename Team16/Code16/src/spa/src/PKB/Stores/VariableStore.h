//
// Created by Isaac Tan on 12/9/23.
//

#ifndef SPA_VARIABLESTORE_H
#define SPA_VARIABLESTORE_H
#include <string>
#include <unordered_set>


class VariableStore {
private:
    typedef std::string variable;
    std::unordered_set<variable> variables;
public:
    VariableStore();

    void addVariables(std::unordered_set<variable> variables);

    std::unordered_set<variable> getVariables();

};


#endif //SPA_VARIABLESTORE_H
