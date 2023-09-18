#pragma once

#include <unordered_set>
#include <string>
#include <unordered_map>

#include "SP/TNode.h"


class TNode;
class ProcedureTNode;
class AssignTNode;
class VariableTNode;
class ConstantTNode;
class PlusTNode;
class MinusTNode;

class Visitor {
 public:
    virtual void visit(const ProcedureTNode* node, std::string& key) = 0;
    virtual void visit(const AssignTNode* node, std::string& key) = 0;
    virtual void visit(const VariableTNode* node, std::string& key) = 0;
    virtual void visit(const ConstantTNode* node, std::string& key) = 0;
    virtual void visit(const PlusTNode* node, std::string& key) = 0;
    virtual void visit(const MinusTNode* node, std::string& key) = 0;
    virtual void visit(const ReadTNode* node, std::string& key) = 0;

    std::unordered_map<std::string, std::unordered_set<std::string>> assignVarHashmap;
    std::unordered_map<std::string, std::unordered_set<std::string>> assignConstHashmap;
    std::unordered_map<int, std::unordered_set<std::string>> usesStatementNumberHashmap;
    std::unordered_map<int, std::string> usesStatementNumberVarHashmap;
    std::unordered_map<int, std::unordered_set<std::string>> constUsesMap;
    std::unordered_map<int, std::unordered_set<std::string>> varUsesMap;
    std::unordered_set<int> assignmentStatementsHashset;
    std::unordered_map<std::string, std::unordered_set<int>> procedureStatementNumberHashmap;
    std::unordered_set<std::string> variablesHashset;
    std::unordered_set<std::string> constantsHashset;
    std::string currKey;

    std::unordered_map<std::string, std::unordered_set<int>> getProcedureStatementNumberHashmap() const {
        return procedureStatementNumberHashmap;
    }

    std::unordered_set<std::string> getVariablesHashset() const {
        return variablesHashset;
    }
    std::unordered_set<std::string> getConstantsHashset() const {
        return constantsHashset;
    }
    std::unordered_map<std::string, std::unordered_set<std::string>> getAssignVarHashmap() const {
        return assignVarHashmap;
    }
    std::unordered_map<std::string, std::unordered_set<std::string>> getAssignConstHashmap() const {
        return assignConstHashmap;
    }
    std::unordered_map<int, std::unordered_set<std::string>> getUsesStatementNumberHashmap() const {
        return usesStatementNumberHashmap;
    }
    std::unordered_map<int, std::string> getUsesStatementNumberVarHashmap() const {
        return usesStatementNumberVarHashmap;
    }
    std::unordered_set<int> getAssignmentStatementsHashset() const {
        return assignmentStatementsHashset;
    }
    std::unordered_map<int, std::unordered_set<std::string>> getLineUsesConst() const {
        return constUsesMap;
    };
    std::unordered_map<int, std::unordered_set<std::string>> getLineUsesVar() const {
        return varUsesMap;
    };
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
};

