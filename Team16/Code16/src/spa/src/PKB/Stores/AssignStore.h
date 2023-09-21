#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "../Helper/Wildcard.h"
#include "../Helper/StmtEntity.h"



class AssignStore {
 private:
    typedef std::string variable;
    typedef std::string partialMatch;
    typedef int statementNumber;
    std::unordered_map<statementNumber, std::unordered_set<partialMatch>> numRHSMap;
    std::unordered_map<statementNumber, variable> numLHSMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> reverseNumLHSMap;
    std::unordered_map<partialMatch, std::unordered_set<statementNumber>> reverseNumRHSMap;

 public:
    AssignStore();

    void addNumRHSMap(std::unordered_map<statementNumber, std::unordered_set<partialMatch>> numRHSMap);

    void addNumLHSMap(std::unordered_map<statementNumber, variable> numLHSMap);

    std::unordered_set<statementNumber> getAllAssigns();

    std::vector<std::pair<statementNumber, variable>> getAssignPair(partialMatch partial);

    std::vector<std::pair<statementNumber, variable>> getAssignPair(Wildcard wildcard);

    std::unordered_set<statementNumber> getAssigns(Wildcard lhs, partialMatch rhs);

    std::unordered_set<statementNumber> getAssigns(Wildcard lhs, Wildcard rhs);

    std::unordered_set<statementNumber> getAssigns(partialMatch lhs, partialMatch rhs);

    std::unordered_set<statementNumber> getAssigns(partialMatch lhs, Wildcard rhs);

};
