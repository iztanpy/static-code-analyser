#pragma once

#ifndef SPA_VISITOR_H
#define SPA_VISITOR_H
#include "TNode.h"
#include <unordered_set>

class TNode;
class AssignTNode;
class VariableTNode;
class ConstantTNode;
class PlusTNode;
class MinusTNode;

using namespace std;

class Visitor {
public:
    virtual void visit(const AssignTNode *node, std::string& key) = 0;
    virtual void visit(const VariableTNode *node, std::string& key) = 0;
    virtual void visit(const ConstantTNode *node, std::string& key) = 0;
    virtual void visit(const PlusTNode *node, std::string& key) = 0;
    virtual void visit(const MinusTNode *node, std::string& key) = 0;

    unordered_map<string, unordered_set<string>> assignVarHashmap;
    unordered_map<string, unordered_set<string>> assignConstHashmap;
    unordered_set<string> variablesHashset;
    unordered_set<string> constantsHashset;
    std::string currKey;

    std::unordered_set<std::string> getVariablesHashset() const {
        std::cout << "Visitor getVariablesHashset called size " << variablesHashset.size() << std::endl;
        return variablesHashset;
    }
    std::unordered_set<std::string> getConstantsHashset() const {
        return constantsHashset;
    }
    unordered_map<string, unordered_set<string>> getAssignVarHashmap() const {
        return assignVarHashmap;
    }
    unordered_map<string, unordered_set<string>> getAssignConstHashmap() const {
        return assignConstHashmap;
    }
};

class ASTVisitor: public Visitor {
public:
    ASTVisitor() = default;

    void visit(const AssignTNode* node, std::string& key) override;
    void visit(const VariableTNode* node, std::string& key) override;
    void visit(const ConstantTNode* node, std::string& key) override;
    void visit(const PlusTNode* node, std::string& key) override;
    void visit(const MinusTNode* node, std::string& key) override;
};
#endif //SPA_VISITOR_H
