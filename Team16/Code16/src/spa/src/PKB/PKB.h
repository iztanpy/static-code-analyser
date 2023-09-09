#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>



class PKB {
 private:
    std::vector<int> assignments;
    std::vector<std::string> variables;
    std::vector<std::string> constants;
    std::unordered_map<std::string, std::unordered_set<std::string>>  UsesConst;
    std::unordered_map<std::string, std::unordered_set<std::string>>  UsesVar;
    std::unordered_map<int, std::unordered_set<std::string>> LineUses;

 public:
     PKB(std::vector<int> assignments,
         std::vector<std::string> variables,
         std::vector<std::string> constants,
         std::unordered_map<std::string, std::unordered_set<std::string>>  UsesConst,
         std::unordered_map<std::string, std::unordered_set<std::string>>  UsesVar,
         std::unordered_map<int, std::unordered_set<std::string>> LineUses);
     PKB();

    void setPKB(std::vector<int> assignments,
                std::vector<std::string> variables,
                std::vector<std::string> constants,
                std::unordered_map<std::string, std::unordered_set<std::string>>  UsesConst,
                std::unordered_map<std::string, std::unordered_set<std::string>>  UsesVar,
                std::unordered_map<int, std::unordered_set<std::string>> LineUses);


     void setAssignments(std::vector<int> assignments);
     void setVariables(std::vector<std::string> variables);
     void setConstants(std::vector<std::string> constants);
     void setUsesConst(std::unordered_map<std::string, std::unordered_set<std::string>>  UsesConst);
     void setUsesVar(std::unordered_map<std::string, std::unordered_set<std::string>>  UsesVar);
     void setLineUses(std::unordered_map<int, std::unordered_set<std::string>> LineUses );

     std::vector<int> getAssignments();
     std::vector<std::string> getVariables();
     std::vector<std::string> getConstants();
     std::unordered_set<std::string> getVariablesUsedBy(int lineNumber);
};
