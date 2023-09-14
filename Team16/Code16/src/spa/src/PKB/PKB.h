#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>

class PKB {
 private:
  std::unordered_set<int> assignments;
  std::unordered_set<std::string> variables;
  std::unordered_set<std::string> constants;
  std::unordered_map<std::string, std::unordered_set<std::string>> UsesConst;
  std::unordered_map<std::string, std::unordered_set<std::string>> UsesVar;
  std::unordered_map<int, std::unordered_set<std::string>> LineUses;

 public:
  PKB(std::unordered_set<int> assignments,
      std::unordered_set<std::string> variables,
      std::unordered_set<std::string> constants,
      std::unordered_map<std::string, std::unordered_set<std::string>> UsesConst,
      std::unordered_map<std::string, std::unordered_set<std::string>> UsesVar,
      std::unordered_map<int, std::unordered_set<std::string>> LineUses);
  PKB();

  void setPKB(std::unordered_set<int> assignments,
              std::unordered_set<std::string> variables,
              std::unordered_set<std::string> constants,
              std::unordered_map<std::string, std::unordered_set<std::string>> UsesConst,
              std::unordered_map<std::string, std::unordered_set<std::string>> UsesVar,
              std::unordered_map<int, std::unordered_set<std::string>> LineUses);

  void setAssignments(std::unordered_set<int> assignments);
  void setVariables(std::unordered_set<std::string> variables);
  void setConstants(std::unordered_set<std::string> constants);
  void setUsesConst(std::unordered_map<std::string, std::unordered_set<std::string>> UsesConst);
  void setUsesVar(std::unordered_map<std::string, std::unordered_set<std::string>> UsesVar);
  void setLineUses(std::unordered_map<int, std::unordered_set<std::string>> LineUses);

  std::unordered_set<int> getAssignments();
  std::unordered_set<std::string> getVariables();
  std::unordered_set<std::string> getConstants();
  std::unordered_set<std::string> getVariablesUsedBy(int lineNumber);

  // Disable copying
  PKB(const PKB&) = delete;
  PKB& operator=(const PKB&) = delete;
};
