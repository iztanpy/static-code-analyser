#pragma once

#include <unordered_set>
#include <string>
#include <unordered_map>
#include <set>
#include <utility>

#include "SP/TNode.h"
#include "utils/entity_types.h"

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


/**
 * @class Visitor
 * @brief Abstract base class for AST node visitors.
 *
 * The `Visitor` class defines an abstract base class for visitors that traverse and analyze the Abstract Syntax Tree (AST).
 * Subclasses of this class implement specific behavior for visiting different types of AST nodes.
 */
class Visitor {
 public:
   /**
    * @brief Visits a ProcedureTNode in the AST.
    *
    * This method is called when traversing a ProcedureTNode in the Abstract Syntax Tree (AST).
    *
    * @param node Pointer to the ProcedureTNode being visited.
    * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
    */
    virtual void visit(const ProcedureTNode* node, std::string& key) = 0;
    /**
     * @brief Visits an AssignTNode in the AST.
     *
     * This method is called when traversing an AssignTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the AssignTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const AssignTNode* node, std::string& key) = 0;
    /**
     * @brief Visits a VariableTNode in the AST.
     *
     * This method is called when traversing a VariableTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the VariableTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const VariableTNode* node, std::string& key) = 0;
    /**
     * @brief Visits a ConstantTNode in the AST.
     *
     * This method is called when traversing a ConstantTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the ConstantTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const ConstantTNode* node, std::string& key) = 0;
    /**
     * @brief Visits a PlusTNode in the AST.
     *
     * This method is called when traversing a PlusTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the PlusTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const PlusTNode* node, std::string& key) = 0;
    /**
     * @brief Visits a MinusTNode in the AST.
     *
     * This method is called when traversing a MinusTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the MinusTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const MinusTNode* node, std::string& key) = 0;
    /**
     * @brief Visits a MultiplyTNode in the AST.
     *
     * This method is called when traversing a MultiplyTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the MultiplyTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const MultiplyTNode* node, std::string& key) = 0;
    /**
     * @brief Visits a DivideTNode in the AST.
     *
     * This method is called when traversing a DivideTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the DivideTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const DivideTNode* node, std::string& key) = 0;
    /**
     * @brief Visits a ModTNode in the AST.
     *
     * This method is called when traversing a ModTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the ModTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const ModTNode* node, std::string& key) = 0;
    /**
     * @brief Visits a CondOperatorTNode in the AST.
     *
     * This method is called when traversing a CondOperatorTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the CondOperatorTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const CondOperatorTNode* node, std::string& key) = 0;
    /**
     * @brief Visits a RelOperatorTNode in the AST.
     *
     * This method is called when traversing a RelOperatorTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the RelOperatorTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const RelOperatorTNode* node, std::string& key) = 0;
    /**
     * @brief Visits a ReadTNode in the AST.
     *
     * This method is called when traversing a ReadTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the ReadTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const ReadTNode* node, std::string& key) = 0;
    /**
     * @brief Visits a WhileTNode in the AST.
     *
     * This method is called when traversing a WhileTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the WhileTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const WhileTNode* node, std::string& key) = 0;
    /**
     * @brief Visits a PrintTNode in the AST.
     *
     * This method is called when traversing a PrintTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the PrintTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const PrintTNode* node, std::string& key) = 0;
    /**
     * @brief Visits an IfTNode in the AST.
     *
     * This method is called when traversing an IfTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the IfTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
    virtual void visit(const IfTNode* node, std::string& key) = 0;
    /**
     * @brief Visits a CallTNode in the AST.
     *
     * This method is called when traversing a CallTNode in the Abstract Syntax Tree (AST).
     *
     * @param node Pointer to the CallTNode being visited.
     * @param key A string key used for visitation, typically indicating the context or purpose of the visit.
     */
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

  // Calls
  std::unordered_map<std::string, std::unordered_set<std::string>> callerCalleeHashmap;
  std::unordered_map<int, std::string> callStatementNumberEntityHashmap;

  // Other
  std::unordered_map<int, StmtEntity> statementTypesMap;
  std::unordered_set<std::string> variables;
  std::unordered_set<std::string> constants;
  std::unordered_map<std::string, std::pair<int, int>> procedureLineNumberHashmap;



  std::set<std::string> procedureLabels;
  std::string currKey;


   /**
    * @brief Get the starting to ending line numbers of a particular procedure. 
    *
    * This method returns an unordered map that associates procedure names with its starting to ending line numbers.
    *
    * @return An unordered map where keys are procedure names, and values are a tuple of statement numbers.
    */
    std::unordered_map<std::string, std::pair<int, int>> getProcedureLineNumberHashmap() const {
      return procedureLineNumberHashmap;
    }

    void setProcedureLineNumberMap(std::string procedureName, int statementNumber) {
        if (procedureLineNumberHashmap[procedureName].first) {
            procedureLineNumberHashmap[procedureName].second = statementNumber;
        } else {
            procedureLineNumberHashmap[procedureName].first = statementNumber;
        }
    }

    std::unordered_map<std::string, std::unordered_set<std::string>> getCallerCalleeHashmap() const {
      return callerCalleeHashmap;
    }

    void setCallerCalleeMap(std::string caller, std::string callee) {
        callerCalleeHashmap[caller].insert(callee);
    }

    std::unordered_map<int, std::string> getCallStatementNumberEntityHashmap() const {
        return callStatementNumberEntityHashmap;
    }

    void setCallStatementNumberEntityHashmap(int statementNumber, std::string entityName) {
        callStatementNumberEntityHashmap[statementNumber] = entityName;
    }

    /**
     * @brief Get the mapping of procedure names to the statement numbers where they are defined.
     *
     * This method returns an unordered map that associates procedure names with sets of statement numbers
     * where those procedures are defined.
     *
     * @return An unordered map where keys are procedure names, and values are sets of statement numbers.
     */
    std::unordered_map<std::string, std::unordered_set<int>> getProcedureStatementNumberHashmap() const {
        return procedureStatementNumberHashmap;
    }
    /**
     * @brief Get the mapping of statement numbers to the set of variables used in those statements.
     *
     * This method returns an unordered map that associates statement numbers with sets of variable names
     * that are used in those statements.
     *
     * @return An unordered map where keys are statement numbers, and values are sets of variable names.
     */
    std::unordered_map<int, std::unordered_set<std::string>> getUsesStatementNumberHashmap() const {
        return usesStatementNumberHashmap;
    }
    /**
     * @brief Get the mapping of statement numbers to patterns of right-hand-side expressions used in those statements.
     *
     * This method returns an unordered map that associates statement numbers with sets of patterns representing
     * the right-hand-side expressions used in those statements.
     *
     * @return An unordered map where keys are statement numbers, and values are sets of right-hand-side patterns.
     */
    std::unordered_map<int, std::unordered_set<std::string>> getUsesLineRHSPatternMap() const {
        return usesLineRHSPatternMap;
    }
    /**
     * @brief Get the mapping of statement numbers to variables modified in those statements.
     *
     * This method returns an unordered map that associates statement numbers with the variable names modified
     * in those statements.
     *
     * @return An unordered map where keys are statement numbers, and values are variable names.
     */
    std::unordered_map<int, std::string> getModifiesMap() const {
        return modifiesMap;
    }
    /**
     * @brief Get the mapping of parent statement numbers to their child statement numbers.
     *
     * This method returns an unordered map that associates parent statement numbers with sets of child statement numbers.
     *
     * @return An unordered map where keys are parent statement numbers, and values are sets of child statement numbers.
     */
    std::unordered_map<int, std::unordered_set<int>> getParentStatementNumberMap() const {
        return parentStatementNumberHashmap;
    }
    /**
     * @brief Get the mapping of statement numbers to their corresponding follow statement numbers.
     *
     * This method returns an unordered map that associates statement numbers with their corresponding follow statement numbers.
     *
     * @return An unordered map where keys are statement numbers, and values are the follow statement numbers.
     */
    std::unordered_map<int, int> getFollowStatementNumberMap() const {
        return followStatementNumberHashmap;
    }
    /**
     * @brief Set the parent-child relationship between statement numbers.
     *
     * This method sets the parent-child relationship between a parent statement number and a child statement number.
     *
     * @param parent The parent statement number.
     * @param child The child statement number.
     */
    void setParentStatementNumberMap(int parent, int child) {
        parentStatementNumberHashmap[parent].insert(child);
    }
    /**
     * @brief Set the follow relationship between statement numbers.
     *
     * This method sets the follow relationship between a statement number before another statement number.
     *
     * @param before The statement number that precedes.
     * @param after The statement number that follows.
     */
    void setFollowStatementNumberMap(int before, int after) {
        followStatementNumberHashmap[before] = after;
    }
    /**
     * @brief Insert a procedure label into the collection.
     *
     * This method inserts a procedure label into the collection of procedure labels.
     *
     * @param label The procedure label to insert.
     */
    void insertProcedureLabel(std::string label) {
        procedureLabels.insert(label);
    }
    /**
     * @brief Get the mapping of statement numbers to the left-hand-side (LHS) variables used in those statements.
     *
     * This method returns an unordered map that associates statement numbers with the left-hand-side (LHS) variable names
     * used in those statements.
     *
     * @return An unordered map where keys are statement numbers, and values are LHS variable names.
     */
    std::unordered_map<int, std::string> getUsesLineLHSMap() const {
        return usesLineLHSMap;
    }
    /**
     * @brief Get the mapping of statement numbers to their corresponding statement entity types.
     *
     * This method returns an unordered map that associates statement numbers with their corresponding statement entity types.
     *
     * @return An unordered map where keys are statement numbers, and values are statement entity types.
     */
    std::unordered_map<int, StmtEntity> getStatementTypesMap() const {
      return statementTypesMap;
    }
    /**
     * @brief Get a set of variable names used in the SIMPLE program.
     *
     * This method returns a set containing variable names used in the SIMPLE program.
     *
     * @return A set of variable names.
     */
    std::unordered_set<std::string> getVariables() const {
        return variables;
    }
    /**
     * @brief Get a set of constant names used in the SIMPLE program.
     *
     * This method returns a set containing constant names used in the SIMPLE program.
     *
     * @return A set of constant names.
     */
    std::unordered_set<std::string> getConstants() const {
        return constants;
    }
    /**
     * @brief Get a set of procedure labels collected during analysis.
     *
     * This method returns a set containing procedure labels collected during the analysis of the code.
     *
     * @return A set of procedure labels.
     */
    std::set<std::string> getProcedureLabels() const {
        return procedureLabels;
    }
    /**
     * @brief Get the mapping of statement numbers to variables on the right-hand side (RHS) of assignments used in those statements.
     *
     * This method returns an unordered map that associates statement numbers with sets of variable names
     * appearing on the right-hand side (RHS) of assignments in those statements.
     *
     * @return An unordered map where keys are statement numbers, and values are sets of RHS variable names.
     */
    std::unordered_map<int, std::unordered_set<std::string>> getUsesLineRHSVarMap() const {
        return usesLineRHSVarMap;
    }
};


/**
 * @class ASTVisitor
 * @brief Concrete visitor for traversing and analyzing the AST.
 *
 * The `ASTVisitor` class is a concrete implementation of the `Visitor` class.
 * It provides specific implementations for visiting various types of AST nodes and collecting analysis results.
 *
 * @remarks
 * AST traversal using the `ASTVisitor` class allows for the collection of various analysis results,
 * such as procedure information, variable usage, control flow relationships, and more.
 */
class ASTVisitor : public Visitor {
 public:
   /**
    * @brief Default constructor for the `ASTVisitor` class.
    *
    * The default constructor initializes an `ASTVisitor` object with default settings.
    */
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
