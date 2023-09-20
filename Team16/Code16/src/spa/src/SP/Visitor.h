#pragma once

#include <unordered_set>
#include <string>
#include <unordered_map>
#include <set>

#include "SP/TNode.h"


class TNode;
class ProcedureTNode;
class AssignTNode;
class VariableTNode;
class ConstantTNode;
class PlusTNode;
class MinusTNode;
class WhileTNode;
class ReadTNode;
class CallTNode;
class PrintTNode;
class IfTNode;

class Visitor {
 public:
    virtual void visit(const ProcedureTNode* node, std::string& key) = 0;
    virtual void visit(const AssignTNode* node, std::string& key) = 0;
    virtual void visit(const VariableTNode* node, std::string& key) = 0;
    virtual void visit(const ConstantTNode* node, std::string& key) = 0;
    virtual void visit(const PlusTNode* node, std::string& key) = 0;
    virtual void visit(const MinusTNode* node, std::string& key) = 0;
    virtual void visit(const ReadTNode* node, std::string& key) = 0;
    virtual void visit(const WhileTNode* node, std::string& key) = 0;
    virtual void visit(const PrintTNode* node, std::string& key) = 0;
    virtual void visit(const IfTNode* node, std::string& key) = 0;
    virtual void visit(const CallTNode* node, std::string& key) = 0;

    // Procedure
    std::unordered_map<std::string, std::unordered_set<int>> procedureStatementNumberHashmap;
    // Uses
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSPatternMap;
    std::unordered_map<int, std::string> usesLineLHSMap;
    std::unordered_map<int, std::unordered_set<std::string>> usesStatementNumberHashmap;
    std::unordered_map<int, std::unordered_set<std::string>> modifiesStatementNumberHashmap;
    std::unordered_set<std::string> variables;
    std::unordered_set<std::string> constants;
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap;
    std::set<std::string> procedureLabels;
    std::string currKey;

    std::unordered_map<std::string, std::unordered_set<int>> getProcedureStatementNumberHashmap() const {
      return procedureStatementNumberHashmap;
    }
    std::unordered_map<int, std::unordered_set<std::string>> getUsesStatementNumberHashmap() const {
        return usesStatementNumberHashmap;
    }
    std::unordered_map<int, std::unordered_set<std::string>> getUsesLineRHSPatternMap() const {
        return usesLineRHSPatternMap;
    }
    std::unordered_map<int, std::unordered_set<std::string>> getModifiesStatementNumberMap() const {
        return modifiesStatementNumberHashmap;
    }
    std::unordered_map<int, std::string> getUsesLineLHSMap() const {
        return usesLineLHSMap;
    }
    std::unordered_set<std::string> getVariables() const {
        return variables;
    }
    std::unordered_set<std::string> getConstants() const {
        return constants;
    }
    std::unordered_map<int, std::unordered_set<std::string>> getUsesLineRHSVarMap() const {
        return usesLineRHSVarMap;
    }
};

class ASTVisitor : public Visitor {
 public:
    ASTVisitor() = default;

    void visit(const ProcedureTNode* node, std::string& key) override;
    void visit(const AssignTNode* node, std::string& key) override;
    void visit(const VariableTNode* node, std::string& key) override;
    void visit(const ConstantTNode* node, std::string& key) override;
    void visit(const PlusTNode* node, std::string& key) override;
    void visit(const MinusTNode* node, std::string& key) override;
    void visit(const ReadTNode* node, std::string& key) override;
    void visit(const WhileTNode* node, std::string& key) override;
    void visit(const PrintTNode* node, std::string& key) override;
    void visit(const IfTNode* node, std::string& key) override;
    void visit(const CallTNode* node, std::string& key) override;
};

