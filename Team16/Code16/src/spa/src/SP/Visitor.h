#pragma once

#include <unordered_set>
#include <string>
#include <unordered_map>
#include <set>

#include "SP/TNode.h"
#include "PKB/Helper/StmtEntity.h"

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
    virtual void visit(const MultiplyTNode* node, std::string& key) = 0;
    virtual void visit(const DivideTNode* node, std::string& key) = 0;
    virtual void visit(const ModTNode* node, std::string& key) = 0;
    virtual void visit(const CondOperatorTNode* node, std::string& key) = 0;
    virtual void visit(const RelOperatorTNode* node, std::string& key) = 0;
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
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap;
    // to remove
    std::unordered_map<int, std::unordered_set<std::string>> usesStatementNumberHashmap;

    // Modifies
    std::unordered_map<int, std::string> modifiesMap;

    // Parent
    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap;

    // Follows
    std::unordered_map<int, int> followStatementNumberHashmap;

    // Other
    std::unordered_map<int, StmtEntity> statementTypesMap;
    std::unordered_set<std::string> variables;
    std::unordered_set<std::string> constants;

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
    std::unordered_map<int, std::string> getModifiesMap() const {
        return modifiesMap;
    }
    std::unordered_map<int, std::unordered_set<int>> getParentStatementNumberMap() const {
        return parentStatementNumberHashmap;
    }
    std::unordered_map<int, int> getFollowStatementNumberMap() const {
        return followStatementNumberHashmap;
    }
    void setParentStatementNumberMap(int parent, int child) {
        parentStatementNumberHashmap[parent].insert(child);
    }
    void setFollowStatementNumberMap(int before, int after) {
        followStatementNumberHashmap[before] = after;
    }

    void insertProcedureLabel(std::string label) {
        procedureLabels.insert(label);
    }
    std::unordered_map<int, std::string> getUsesLineLHSMap() const {
        return usesLineLHSMap;
    }
    std::unordered_map<int, StmtEntity> getStatementTypesMap() const {
      return statementTypesMap;
    }
    std::unordered_set<std::string> getVariables() const {
        return variables;
    }
    std::unordered_set<std::string> getConstants() const {
        return constants;
    }
    std::set<std::string> getProcedureLabels() const {
        return procedureLabels;
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
    void visit(const MultiplyTNode* node, std::string& key) override;
    void visit(const DivideTNode* node, std::string& key) override;
    void visit(const ModTNode* node, std::string& key) override;
    void visit(const CondOperatorTNode* node, std::string& key) override;
    void visit(const RelOperatorTNode* node, std::string& key) override;
    void visit(const ReadTNode* node, std::string& key) override;
    void visit(const WhileTNode* node, std::string& key) override;
    void visit(const PrintTNode* node, std::string& key) override;
    void visit(const IfTNode* node, std::string& key) override;
    void visit(const CallTNode* node, std::string& key) override;
};
