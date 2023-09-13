//
// Created by Isaac Tan on 13/9/23.
//

#ifndef SPA_ASSIGNSTORE_H
#define SPA_ASSIGNSTORE_H

#include <string>
#include <unordered_set>


class AssignStore {
private:
    typedef std::string variable;
    typedef std::string possibleCombinations;
    typedef int statementNumber;
    std::unordered_map<statementNumber, std::unordered_set<possibleCombinations>> numRHSMap;
    std::unordered_map<statementNumber, variable> numLHSMap;

public:
    AssignStore();

    void addNumRHSMap(std::unordered_map<statementNumber, std::unordered_set<possibleCombinations>> numRHSMap);

    void addNumLHSMap(std::unordered_map<statementNumber, variable> numLHSMap);

    std::unordered_set<statementNumber> getAllAssigns();

    std::unordered_set<statementNumber> getAssigns(variable LHS, possibleCombinations RHS);



};


#endif //SPA_ASSIGNSTORE_H
