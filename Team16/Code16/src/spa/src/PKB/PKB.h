#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>


class PKB {
 private:
     std::vector<int> assignments;
     std::vector<std::string> variables;
     std::vector<std::string> constants;

     void setPKB(std::vector<int> assignments, std::vector<std::string> variables, std::vector<std::string> constants);


 public:
     PKB(std::vector<int> assignments, std::vector<std::string> variables, std::vector<std::string> constants);
     PKB();

     void setAssignments(std::vector<int> assignments);
     void setVariables(std::vector<std::string> variables);
     void setConstants(std::vector<std::string> constants);

     std::vector<int> getAssignments();
     std::vector<std::string> getVariables();
     std::vector<std::string> getConstants();
};
