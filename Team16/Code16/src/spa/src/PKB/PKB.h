#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <utility>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include "Stores/VariableStore.h"
#include "Stores/AssignStore.h"
#include "Stores/ConstantStore.h"
#include "Stores/StatementStore.h"
#include "Stores/ParentStore.h"
#include "Stores/FollowsStore.h"
#include "Stores/RelationStore.h"
#include "Stores/ProcedureStore.h"
#include "Stores/IfStore.h"
#include "Stores/WhileStore.h"
#include "Stores/CallStore.h"
#include "Stores/NextStore.h"
#include "utils/entity_types.h"
#include "utils/clauses_types.h"
#include "utils/hash_utils.h"

typedef std::string variable;
typedef int statementNumber;
typedef std::string full;
typedef std::string constant;
typedef std::string statementType;
typedef std::string partialMatch;
typedef std::string procedure;

class PKB {
 private:
    std::unique_ptr<AssignStore> assignStore;
    std::unique_ptr<VariableStore> variableStore;
    std::unique_ptr<RelationStore> usesStore;
    std::unique_ptr<ConstantStore> constantStore;
    std::unique_ptr<StatementStore> statementStore;
    std::unique_ptr<ParentStore> parentStore;
    std::unique_ptr<FollowsStore> followsStore;
    std::unique_ptr<RelationStore> modifiesStore;
    std::unique_ptr<ProcedureStore> procedureStore;
    std::unique_ptr<WhileStore> whileStore;
    std::unique_ptr<IfStore> ifStore;
    std::unique_ptr<CallStore> callStore;
    std::unique_ptr<NextStore> nextStore;

 public:
    PKB();

    // General methods
    std::unordered_set<statementNumber> getCommonStatements(std::unordered_set<statementNumber> set1,
        std::unordered_set<statementNumber> set2);

    // AssignStore methods

    /**
    * @brief Sets assignment information in the program.
    *
    * This method sets assignment information, including mappings of statement numbers to sets of possible combinations
    * (numRHSMap) and statement numbers to variables (numLHSMap) in the program.
    *
    * @param numRHSMap An unordered map of statement numbers to sets of possible combinations for the right-hand side of assignments.
    * @param numLHSMap An unordered map of statement numbers to variables for the left-hand side of assignments.
    */
    void setAssignments(std::unordered_map<statementNumber, std::unordered_set<partialMatch>> partialRHSMap,
        std::unordered_map<statementNumber, variable> numLHSMap);

    void setAssignments(std::unordered_map<statementNumber, std::unordered_set<partialMatch>> partialRHSMap,
        std::unordered_map<statementNumber, full> fullRHSMap,
        std::unordered_map<statementNumber, variable> numLHSMap);

    std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPairPartial(partialMatch partial);
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPairFull(full full);
    std::unordered_set<statementNumber> getAssignsWcF(Wildcard lhs, full rhs);
    std::unordered_set<statementNumber> getAssignsFF(full lhs, full rhs);

    /**
    * @brief Retrieves all assignment statement numbers in the program.
    *
    * This method returns an unordered set containing all statement numbers corresponding to assignment statements in the program.
    *
    * @return An unordered set of statement numbers representing assignment statements.
    */
    std::unordered_set<statementNumber> getAllAssigns();

    /**
    * @brief Retrieves pairs of statement numbers and variables for assignment statements that partially match a given pattern.
    *
    * This method returns an unordered set of pairs, where each pair consists of a statement number and a variable,
    * for assignment statements that partially match the given pattern (partialMatch).
    *
    * @param partial The partial match pattern for assignment statements.
    * @return An unordered set of pairs representing assignment statements that partially match the pattern.
    */
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPair(partialMatch partial);

    /**
    * @brief Retrieves pairs of statement numbers and variables for assignment statements that match a wildcard pattern.
    *
    * This method returns an unordered set of pairs, where each pair consists of a statement number and a variable,
    * for assignment statements that match the given wildcard pattern (Wildcard).
    *
    * @param wildcard The wildcard pattern for assignment statements.
    * @return An unordered set of pairs representing assignment statements that match the wildcard pattern.
    */
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPair(Wildcard wildcard);

    /**
    * @brief Retrieves assignment statement numbers that match a pattern for the left-hand side (LHS) and partial match for the right-hand side (RHS).
    *
    * This method returns an unordered set of statement numbers for assignment statements that match the pattern for the LHS
    * and partially match the pattern for the RHS.
    *
    * @param lhs The pattern for the left-hand side of assignment statements.
    * @param rhs The partial match pattern for the right-hand side of assignment statements.
    * @return An unordered set of statement numbers representing matching assignment statements.
    */
    std::unordered_set<statementNumber> getAssigns(Wildcard lhs, partialMatch rhs);

    /**
    * @brief Retrieves assignment statement numbers that match wildcard patterns for both the left-hand side (LHS) and right-hand side (RHS).
    *
    * This method returns an unordered set of statement numbers for assignment statements that match wildcard patterns for both
    * the LHS and RHS.
    *
    * @param lhs The wildcard pattern for the left-hand side of assignment statements.
    * @param rhs The wildcard pattern for the right-hand side of assignment statements.
    * @return An unordered set of statement numbers representing matching assignment statements.
    */
    std::unordered_set<statementNumber> getAssigns(Wildcard lhs, Wildcard rhs);

    /**
    * @brief Retrieves assignment statement numbers that partially match patterns for both the left-hand side (LHS) and right-hand side (RHS).
    *
    * This method returns an unordered set of statement numbers for assignment statements that partially match patterns for both
    * the LHS and RHS.
    *
    * @param lhs The partial match pattern for the left-hand side of assignment statements.
    * @param rhs The partial match pattern for the right-hand side of assignment statements.
    * @return An unordered set of statement numbers representing matching assignment statements.
    */
    std::unordered_set<statementNumber> getAssigns(partialMatch lhs, partialMatch rhs);

    /**
    * @brief Retrieves assignment statement numbers that partially match a pattern for the left-hand side (LHS) and match a wildcard pattern for the right-hand side (RHS).
    *
    * This method returns an unordered set of statement numbers for assignment statements that partially match a pattern for the LHS
    * and match a wildcard pattern for the RHS.
    *
    * @param lhs The partial match pattern for the left-hand side of assignment statements.
    * @param rhs The wildcard pattern for the right-hand side of assignment statements.
    * @return An unordered set of statement numbers representing matching assignment statements.
    */
    std::unordered_set<statementNumber> getAssigns(partialMatch lhs, Wildcard rhs);


    // ProcedureStore methods

    /**
    * @brief Adds a set of procedures to the program's procedure store.
    *
    * This method adds a set of procedures to the program's procedure store.
    *
    * @param procedures A set of procedures to be added to the program's procedure store.
    */
    void addProcedures(std::set<procedure> procedures);

    /**
    * @brief Adds procedures and their respective start numbers and line numbers to the program's procedure store.
    *
    * This method adds procedures and their respective start numbers and line numbers to the program's procedure store.
    *
    * @param procedures An unordered map of procedures to pairs of start numbers and line numbers.
    */
    void addProcedures(std::unordered_map<procedure, std::pair<int, int>> procedures);

    /**
    * @brief Retrieves all procedures stored in the program.
    *
    * This method returns an unordered set containing all procedures stored in the program's procedure store.
    *
    * @return An unordered set of procedures stored in the program.
    */
    std::unordered_set<procedure> getProcedures();

    /**
    * @brief Retrieves the start number and end number for a specific procedure.
    *
    * This method returns a pair of integers representing the start number and end number for the specified procedure.
    *
    * @param proc The procedure for which to retrieve the start number and end number.
    *
    * @return A pair of integers representing the start number and end number for the specified procedure.
    */
    std::pair<int, int> getProcedureRange(procedure proc);

    /**
    * @brief Adds a set of variables to the program's variable store.
    *
    * This method adds a set of variables to the program's variable store.
    *
    * @param variables An unordered set of variables to be added to the program's variable store.
    */
    void addVariables(std::unordered_set<variable> variables);

    /**
    * @brief Retrieves all variables stored in the program.
    *
    * This method returns an unordered set containing all variables stored in the program's variable store.
    *
    * @return An unordered set of variables stored in the program.
    */
    std::unordered_set<variable> getVariables();


    // UsesStore methods

    /**
    * @brief Stores uses information in the program.
    *
    * This method stores uses information, including mappings of statement numbers to sets of used variables (varUsesMap).
    *
    * @param varUsesMap An unordered map of statement numbers to sets of variables that are used.
    */
    void storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap);

    void storeUsesProcedures(std::unordered_map<procedure, std::pair<int, int>> procedures,
        std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar);

    void storeUsesCalls(std::unordered_map<statementNumber, procedure> calls);

    /**
    * @brief Checks if a specific statement uses a given variable.
    *
    * This method checks whether the statement specified by 'lineNumber' uses the variable 'variableName'.
    *
    * @param lineNumber The statement number to check for variable usage.
    * @param variableName The name of the variable to check for usage.
    * @return true if the statement uses the variable, false otherwise.
    */
    bool isUses(statementNumber lineNumber, variable variableName);

    /**
    * @brief Checks if a specific statement uses variables matching a wildcard pattern.
    *
    * This method checks whether the statement specified by 'lineNumber' uses variables that match the given wildcard pattern.
    *
    * @param lineNumber The statement number to check for variable usage.
    * @param wildcard A wildcard pattern to match against used variables.
    * @return true if the statement uses matching variables, false otherwise.
    */
    bool isUses(statementNumber lineNumber, Wildcard wildcard);

    /**
    * @brief Retrieves variables used by a specific statement.
    *
    * This method returns an unordered set of variables used by the statement specified by 'line'.
    *
    * @param line The statement number for which to retrieve used variables.
    * @return An unordered set of variables used by the specified statement.
    */
    std::unordered_set<variable> uses(statementNumber line);

    /**
    * @brief Retrieves statements that use a specific variable of a given type.
    *
    * This method returns an unordered set of statement numbers for statements that use the variable 'variableName' of the specified type (StmtEntity).
    *
    * @param type The type of statement for which to search for variable usage.
    * @param variableName The name of the variable to search for in statement usage.
    * @return An unordered set of statement numbers that use the specified variable of the given type.
    */
    std::unordered_set<statementNumber> uses(StmtEntity type, variable variableName);

    /**
    * @brief Retrieves statements that use variables matching a wildcard pattern of a given type.
    *
    * This method returns an unordered set of statement numbers for statements that use variables matching the given wildcard pattern (Wildcard) of the specified type (StmtEntity).
    *
    * @param type The type of statement for which to search for variable usage.
    * @param wildcard A wildcard pattern to match against used variables.
    * @return An unordered set of statement numbers that use matching variables of the given type.
    */
    std::unordered_set<statementNumber> uses(StmtEntity type, Wildcard wildcard);

    /**
    * @brief Retrieves pairs of statement numbers and variables for statements of a given type that represent usage relationships.
    *
    * This method returns an unordered set of pairs, where each pair consists of a statement number and a variable, for statements of the specified type (StmtEntity) that represent usage relationships.
    *
    * @param type The type of statement for which to retrieve usage relationships.
    * @return An unordered set of pairs representing usage relationships for statements of the specified type.
    */
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> uses(StmtEntity type);

    bool isUses(procedure procedure, Wildcard wildcard);

    std::unordered_set<variable> uses(procedure procedure);

    bool isUses(procedure procedure, variable variableName);

    std::unordered_set<procedure> usesProcedure(Wildcard wildcard);

    std::unordered_set<procedure> usesProcedure(variable variableName);

    std::unordered_set<std::pair<procedure, variable>, PairHash> usesProcedure();

    // ModifiesStore methods

    /**
    * @brief Stores modifies information in the program.
    *
    * This method stores modifies information, including mappings of statement numbers to modified variables (varModifiesMap).
    *
    * @param varModifiesMap An unordered map of statement numbers to variables that are modified.
    */
    void storeModifies(std::unordered_map<statementNumber, variable> varModifiesMap);

    void storeModifiesProcedures(std::unordered_map<procedure, std::pair<int, int>> procedures,
        std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar);

    void storeModifiesCalls(std::unordered_map<statementNumber, procedure> calls);

    /**
    * @brief Checks if a specific statement modifies a given variable.
    *
    * This method checks whether the statement specified by 'lineNumber' modifies the variable 'variableName'.
    *
    * @param lineNumber The statement number to check for variable modification.
    * @param variableName The name of the variable to check for modification.
    * @return true if the statement modifies the variable, false otherwise.
    */
    bool isModifies(statementNumber lineNumber, variable variableName);

    /**
    * @brief Checks if a specific statement modifies variables matching a wildcard pattern.
    *
    * This method checks whether the statement specified by 'lineNumber' modifies variables that match the given wildcard pattern.
    *
    * @param lineNumber The statement number to check for variable modification.
    * @param wildcard A wildcard pattern to match against modified variables.
    * @return true if the statement modifies matching variables, false otherwise.
    */
    bool isModifies(statementNumber lineNumber, Wildcard wildcard);

    /**
    * @brief Retrieves variables modified by a specific statement.
    *
    * This method returns an unordered set of variables modified by the statement specified by 'line'.
    *
    * @param line The statement number for which to retrieve modified variables.
    * @return An unordered set of variables modified by the specified statement.
    */
    std::unordered_set<variable> relates(statementNumber line);

    /**
    * @brief Retrieves statements that modify a specific variable of a given type.
    *
    * This method returns an unordered set of statement numbers for statements that modify the variable 'variableName' of the specified type (StmtEntity).
    *
    * @param type The type of statement for which to search for variable modification.
    * @param variableName The name of the variable to search for in statement modification.
    * @return An unordered set of statement numbers that modify the specified variable of the given type.
    */
    std::unordered_set<statementNumber> relates(StmtEntity type, variable variableName);

    /**
    * @brief Retrieves statements that modify variables matching a wildcard pattern of a given type.
    *
    * This method returns an unordered set of statement numbers for statements that modify variables matching the given wildcard pattern (Wildcard) of the specified type (StmtEntity).
    *
    * @param type The type of statement for which to search for variable modification.
    * @param wildcard A wildcard pattern to match against modified variables.
    * @return An unordered set of statement numbers that modify matching variables of the given type.
    */
    std::unordered_set<statementNumber> relates(StmtEntity type, Wildcard wildcard);

    /**
    * @brief Retrieves pairs of statement numbers and variables for statements of a given type that represent modification relationships.
    *
    * This method returns an unordered set of pairs, where each pair consists of a statement number and a variable, for statements of the specified type (StmtEntity) that represent modification relationships.
    *
    * @param type The type of statement for which to retrieve modification relationships.
    * @return An unordered set of pairs representing modification relationships for statements of the specified type.
    */
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> relates(StmtEntity type);

    bool isModifies(procedure procedure, Wildcard wildcard);

    std::unordered_set<variable> relates(procedure procedure);

    bool isModifies(procedure procedure, variable variableName);

    std::unordered_set<procedure> relatesProcedure(Wildcard wildcard);

    std::unordered_set<procedure> relatesProcedure(variable variableName);

    std::unordered_set<std::pair<procedure, variable>, PairHash> relatesProcedure();

    // ConstantStore methods

    /**
    * @brief Adds a set of constants to the program's constant store.
    *
    * This method adds a set of constants to the program's constant store.
    *
    * @param constants An unordered set of constants to be added to the program's constant store.
    */
    void addConstants(std::unordered_set<constant> constants);

    /**
    * @brief Retrieves all constants stored in the program.
    *
    * This method returns an unordered set containing all constants stored in the program's constant store.
    *
    * @return An unordered set of constants stored in the program.
    */
    std::unordered_set<constant> getConstants();


    // StatementStore methods

    /**
    * @brief Adds statement information to the program.
    *
    * This method adds statement information, including mappings of statement numbers to statement entities (typeMap) in the program.
    *
    * @param typeMap An unordered map of statement numbers to statement entities.
    */
    void addStatements(std::unordered_map<statementNumber, StmtEntity> typeMap);

    /**
    * @brief Retrieves statement numbers of a specific type.
    *
    * This method returns an unordered set containing statement numbers of the specified type (StmtEntity) stored in the program.
    *
    * @param type The type of statement for which to retrieve statement numbers.
    * @return An unordered set of statement numbers of the specified type.
    */
    std::unordered_set<statementNumber> getStatements(StmtEntity type);


    // ParentStore methods

    /**
    * @brief Stores parent relationship information in the program.
    *
    * This method stores parent relationship information, including mappings of statement numbers to sets of parent statements (map).
    *
    * @param map An unordered map of statement numbers to sets of parent statement numbers.
    */
    void storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map);

    /**
    * @brief Retrieves parent statement numbers based on a wildcard pattern and statement type.
    *
    * This method returns an unordered set of parent statement numbers that match a wildcard pattern (wildcard) and the specified statement type (entity).
    *
    * @param wildcard A wildcard pattern for statement numbers.
    * @param entity The type of statement for which to search for parent statements.
    * @return An unordered set of parent statement numbers that match the pattern and type.
    */
    std::unordered_set<statementNumber> parent(Wildcard wildcard, StmtEntity entity);

    /**
    * @brief Retrieves parent statement numbers based on a specific statement number and statement type.
    *
    * This method returns an unordered set of parent statement numbers for the specified statement number (num) and the specified statement type (entity).
    *
    * @param num The statement number for which to find parent statements.
    * @param entity The type of statement for which to search for parent statements.
    * @return An unordered set of parent statement numbers for the specified statement number and type.
    */
    std::unordered_set<statementNumber> parent(statementNumber num, StmtEntity entity);

    /**
    * @brief Retrieves parent statement numbers based on a statement type and a specific statement number.
    *
    * This method returns an unordered set of parent statement numbers for the specified statement type (entity) and the given statement number (num).
    *
    * @param entity The type of statement for which to search for parent statements.
    * @param num The statement number for which to find parent statements.
    * @return An unordered set of parent statement numbers for the specified type and statement number.
    */
    std::unordered_set<statementNumber> parent(StmtEntity entity, statementNumber num);

    /**
    * @brief Retrieves parent statement numbers based on a statement type and a wildcard pattern.
    *
    * This method returns an unordered set of parent statement numbers for the specified statement type (entity) and a wildcard pattern (wildcard).
    *
    * @param entity The type of statement for which to search for parent statements.
    * @param wildcard A wildcard pattern for statement numbers.
    * @return An unordered set of parent statement numbers for the specified type and wildcard pattern.
    */
    std::unordered_set<statementNumber> parent(StmtEntity entity, Wildcard wildcard);

    /**
    * @brief Retrieves parent statement numbers based on a wildcard pattern and a specific statement number.
    *
    * This method returns an unordered set of parent statement numbers that match a wildcard pattern (wildcard) and the specified statement number (statement).
    *
    * @param wildcard A wildcard pattern for statement numbers.
    * @param statement The specific statement number for which to search for parent statements.
    * @return An unordered set of parent statement numbers that match the pattern and the specific statement number.
    */
    std::unordered_set<statementNumber> parent(Wildcard wildcard, statementNumber statement);

    /**
    * @brief Retrieves parent statement numbers based on a specific statement number and a wildcard pattern.
    *
    * This method returns an unordered set of parent statement numbers for the specified statement number (statement) and a wildcard pattern (wildcard).
    *
    * @param statement The specific statement number for which to find parent statements.
    * @param wildcard A wildcard pattern for statement numbers.
    * @return An unordered set of parent statement numbers for the specific statement number and wildcard pattern.
    */
    std::unordered_set<statementNumber> parent(statementNumber statement, Wildcard wildcard);

    /**
    * @brief Retrieves pairs of parent and child statement numbers based on statement types.
    *
    * This method returns an unordered set of pairs, where each pair consists of a parent statement number and a child statement number,
    * for specified statement types (entity and entity2) representing parent-child relationships.
    *
    * @param entity The first type of statement for which to retrieve parent-child relationships.
    * @param entity2 The second type of statement for which to retrieve parent-child relationships.
    * @return An unordered set of pairs representing parent-child relationships for the specified statement types.
    */
    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
        parent(StmtEntity entity, StmtEntity entity2);

    /**
    * @brief Checks if one statement is the parent of another statement.
    *
    * This method checks if the statement specified by 'parent' is the parent of the statement specified by 'child'.
    *
    * @param parent The statement number to check as the parent.
    * @param child The statement number to check as the child.
    * @return true if 'parent' is the parent of 'child', false otherwise.
    */
    bool isParent(statementNumber parent, statementNumber child);

    /**
    * @brief Checks if a statement, specified by its number, is a parent of statements matching a wildcard pattern and a given type.
    *
    * This method checks if the statement specified by 'parent' is a parent of statements matching a wildcard pattern (wildcard)
    * and the specified statement type (entity).
    *
    * @param parent The statement number to check as the parent.
    * @param wildcard A wildcard pattern for child statements.
    * @return true if 'parent' is a parent of matching child statements, false otherwise.
    */
    bool isParent(statementNumber parent, Wildcard wildcard);

    /**
    * @brief Checks if a statement, specified by its number, is a parent of statements matching a wildcard pattern and a given type.
    *
    * This method checks if the statement specified by 'parent' is a parent of statements matching a wildcard pattern (wildcard)
    * and the specified statement type (entity).
    *
    * @param wildcard A wildcard pattern for parent statements.
    * @param child The statement number to check as the child.
    * @return true if 'parent' is a parent of matching child statements, false otherwise.
    */
    bool isParent(Wildcard wildcard, statementNumber child);

    /**
    * @brief Checks if statements matching a wildcard pattern are parents of statements matching another wildcard pattern.
    *
    * This method checks if statements matching the first wildcard pattern (wildcard) are parents of statements matching
    * the second wildcard pattern (wildcard2).
    *
    * @param wildcard A wildcard pattern for parent statements.
    * @param wildcard2 A wildcard pattern for child statements.
    * @return true if statements matching the first wildcard pattern are parents of statements matching the second wildcard pattern, false otherwise.
    */
    bool isParent(Wildcard wildcard, Wildcard wildcard2);

    /**
    * @brief Retrieves all descendant statements of a specific statement that match a wildcard pattern.
    *
    * This method returns an unordered set of descendant statement numbers that match the specified wildcard pattern (wildcard).
    * Descendants are statements that are indirectly or directly nested within the given statement number (statement).
    *
    * @param statement The statement number for which to find descendant statements.
    * @param wildcard A wildcard pattern for descendant statements.
    * @return An unordered set of descendant statement numbers that match the wildcard pattern.
    */
    std::unordered_set<statementNumber> parentStar(statementNumber statement, Wildcard wildcard);

    /**
    * @brief Retrieves all ancestor statements of a specific statement that match a wildcard pattern.
    *
    * This method returns an unordered set of ancestor statement numbers that match the specified wildcard pattern (wildcard).
    * Ancestors are statements that directly or indirectly enclose the given statement number (statement).
    *
    * @param wildcard A wildcard pattern for ancestor statements.
    * @param statement The statement number for which to find ancestor statements.
    * @return An unordered set of ancestor statement numbers that match the wildcard pattern.
    */
    std::unordered_set<statementNumber> parentStar(Wildcard wildcard, statementNumber statement);

    /**
    * @brief Retrieves all descendant statements of statements matching a wildcard pattern and a specified type.
    *
    * This method returns an unordered set of descendant statement numbers that match the specified wildcard pattern (wildcard)
    * and the specified statement type (entity).
    * Descendants are statements that are indirectly or directly nested within the matching statements.
    *
    * @param wildcard A wildcard pattern for matching statements.
    * @param entity The type of statement for which to search for descendant statements.
    * @return An unordered set of descendant statement numbers that match the wildcard pattern and type.
    */
    std::unordered_set<statementNumber> parentStar(Wildcard wildcard, StmtEntity entity);

    /**
    * @brief Retrieves all ancestor statements of a specific statement number and a specified type that match a wildcard pattern.
    *
    * This method returns an unordered set of ancestor statement numbers that match the specified wildcard pattern (wildcard)
    * and the specified statement type (entity).
    * Ancestors are statements that directly or indirectly enclose the given statement number (num).
    *
    * @param num The statement number for which to find ancestor statements.
    * @param entity The type of statement for which to search for ancestor statements.
    * @return An unordered set of ancestor statement numbers that match the wildcard pattern and type.
    */
    std::unordered_set<statementNumber> parentStar(statementNumber num, StmtEntity entity);

    /**
    * @brief Retrieves all descendant statements of statements matching a specified type and a specific statement number.
    *
    * This method returns an unordered set of descendant statement numbers for the specified statement type (entity)
    * and the given statement number (num).
    * Descendants are statements that are indirectly or directly nested within the matching statements.
    *
    * @param entity The type of statement for which to search for descendant statements.
    * @param num The statement number for which to find descendant statements.
    * @return An unordered set of descendant statement numbers that match the specified type and statement number.
    */
    std::unordered_set<statementNumber> parentStar(StmtEntity entity, statementNumber num);

    /**
    * @brief Retrieves all descendant statements of statements matching a specified type and a wildcard pattern.
    *
    * This method returns an unordered set of descendant statement numbers for the specified statement type (entity)
    * and the given wildcard pattern (wildcard).
    * Descendants are statements that are indirectly or directly nested within the matching statements.
    *
    * @param entity The type of statement for which to search for descendant statements.
    * @param wildcard A wildcard pattern for matching statements.
    * @return An unordered set of descendant statement numbers that match the specified type and wildcard pattern.
    */
    std::unordered_set<statementNumber> parentStar(StmtEntity entity, Wildcard wildcard);

    /**
    * @brief Retrieves pairs of ancestor and descendant statement numbers based on statement types.
    *
    * This method returns an unordered set of pairs, where each pair consists of an ancestor statement number and a descendant statement number,
    * for specified statement types (entity and entity2) representing ancestor-descendant relationships.
    *
    * @param entity The first type of statement for which to retrieve ancestor-descendant relationships.
    * @param entity2 The second type of statement for which to retrieve ancestor-descendant relationships.
    * @return An unordered set of pairs representing ancestor-descendant relationships for the specified statement types.
    */
    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
        parentStar(StmtEntity entity, StmtEntity entity2);

    /**
    * @brief Checks if one statement is an ancestor of another statement.
    *
    * This method checks if the statement specified by 'parent' is an ancestor of the statement specified by 'child'.
    *
    * @param parent The statement number to check as the ancestor.
    * @param child The statement number to check as the descendant.
    * @return true if 'parent' is an ancestor of 'child', false otherwise.
    */
    bool isParentStar(statementNumber parent, statementNumber child);

    /**
    * @brief Checks if a statement, specified by its number, is an ancestor of statements matching a wildcard pattern and a given type.
    *
    * This method checks if the statement specified by 'parent' is an ancestor of statements matching a wildcard pattern (wildcard)
    * and the specified statement type (entity).
    *
    * @param parent The statement number to check as the ancestor.
    * @param wildcard A wildcard pattern for descendant statements.
    * @return true if 'parent' is an ancestor of matching descendant statements, false otherwise.
    */
    bool isParentStar(statementNumber parent, Wildcard wildcard);

    /**
    * @brief Checks if statements matching a wildcard pattern are ancestors of a statement specified by its number.
    *
    * This method checks if statements matching the wildcard pattern (wildcard) are ancestors of the statement specified by 'child'.
    *
    * @param wildcard A wildcard pattern for ancestor statements.
    * @param child The statement number to check as the descendant.
    * @return true if matching ancestor statements are ancestors of 'child', false otherwise.
    */
    bool isParentStar(Wildcard wildcard, statementNumber child);

    /**
    * @brief Checks if statements matching a wildcard pattern are ancestors of statements matching another wildcard pattern.
    *
    * This method checks if statements matching the first wildcard pattern (wildcard) are ancestors of statements matching
    * the second wildcard pattern (wildcard2).
    *
    * @param wildcard A wildcard pattern for ancestor statements.
    * @param wildcard2 A wildcard pattern for descendant statements.
    * @return true if matching ancestor statements are ancestors of matching descendant statements, false otherwise.
    */
    bool isParentStar(Wildcard wildcard, Wildcard wildcard2);


    // FollowStore Method

    /**
    * @brief Stores follows relationship information in the program.
    *
    * This method stores follows relationship information, including mappings of statement numbers to their following statement numbers (map).
    *
    * @param map An unordered map of statement numbers to their following statement numbers.
    */
    void storeFollows(std::unordered_map<statementNumber, statementNumber> map);

    /**
    * @brief Retrieves statements following a wildcard pattern and a specified statement type.
    *
    * This method returns an unordered set of statement numbers that follow the specified wildcard pattern (wildcard)
    * and the specified statement type (entity).
    *
    * @param wildcard A wildcard pattern for preceding statements.
    * @param entity The type of statement for which to search for following statements.
    * @return An unordered set of statement numbers that match the wildcard pattern and type.
    */
    std::unordered_set<statementNumber> follows(Wildcard wildcard, StmtEntity entity);

    /**
    * @brief Retrieves statements following a specific statement number and a specified statement type.
    *
    * This method returns an unordered set of statement numbers that follow the specified statement number (num)
    * and the specified statement type (entity).
    *
    * @param num The statement number for which to find following statements.
    * @param entity The type of statement for which to search for following statements.
    * @return An unordered set of statement numbers that follow the specified statement number and type.
    */
    std::unordered_set<statementNumber> follows(statementNumber num, StmtEntity entity);

    /**
    * @brief Retrieves statements followed by a specific statement number and a specified statement type.
    *
    * This method returns an unordered set of statement numbers that are followed by the specified statement number (num)
    * and the specified statement type (entity).
    *
    * @param entity The type of statement for which to search for preceding statements.
    * @param num The statement number for which to find preceding statements.
    * @return An unordered set of statement numbers that are followed by the specified statement number and type.
    */
    std::unordered_set<statementNumber> follows(StmtEntity entity, statementNumber num);

    /**
    * @brief Retrieves statements followed by statements matching a specified type and a wildcard pattern.
    *
    * This method returns an unordered set of statement numbers that are followed by statements matching the specified statement type (entity)
    * and the given wildcard pattern (wildcard).
    *
    * @param entity The type of statement for which to search for preceding statements.
    * @param wildcard A wildcard pattern for following statements.
    * @return An unordered set of statement numbers that are followed by matching statements of the specified type.
    */
    std::unordered_set<statementNumber> follows(StmtEntity entity, Wildcard wildcard);

    /**
    * @brief Retrieves pairs of preceding and following statement numbers based on statement types.
    *
    * This method returns an unordered set of pairs, where each pair consists of a preceding statement number and a following statement number,
    * for specified statement types (entity1 and entity2) representing follows relationships.
    *
    * @param entity1 The type of statement for which to retrieve preceding statements.
    * @param entity2 The type of statement for which to retrieve following statements.
    * @return An unordered set of pairs representing follows relationships for the specified statement types.
    */
    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
        follows(StmtEntity entity1, StmtEntity entity2);

    /**
    * @brief Checks if one statement follows another statement.
    *
    * This method checks if the statement specified by 'statement1' follows the statement specified by 'statement2'.
    *
    * @param statement1 The statement number to check as the following statement.
    * @param statement2 The statement number to check as the preceding statement.
    * @return true if 'statement1' follows 'statement2', false otherwise.
    */
    bool isFollow(statementNumber statement1, statementNumber statement2);

    /**
    * @brief Checks if a statement, specified by its number, follows statements matching a wildcard pattern and a given type.
    *
    * This method checks if the statement specified by 'statement1' follows statements matching a wildcard pattern (wildcard)
    * and the specified statement type (entity).
    *
    * @param statement1 The statement number to check as the following statement.
    * @param wildcard A wildcard pattern for preceding statements.
    * @return true if 'statement1' follows matching preceding statements, false otherwise.
    */
    bool isFollow(statementNumber statement1, Wildcard wildcard);

    /**
    * @brief Checks if statements matching a wildcard pattern are followed by a statement specified by its number.
    *
    * This method checks if statements matching the wildcard pattern (wildcard) are followed by the statement specified by 'statement1'.
    *
    * @param wildcard A wildcard pattern for following statements.
    * @param statement1 The statement number to check as the following statement.
    * @return true if matching preceding statements are followed by 'statement1', false otherwise.
    */
    bool isFollow(Wildcard wildcard, statementNumber statement1);

    /**
    * @brief Checks if statements matching a wildcard pattern are followed by statements matching another wildcard pattern.
    *
    * This method checks if statements matching the first wildcard pattern (wildcard) are followed by statements matching
    * the second wildcard pattern (wildcard2).
    *
    * @param wildcard A wildcard pattern for preceding statements.
    * @param wildcard2 A wildcard pattern for following statements.
    * @return true if matching preceding statements are followed by matching following statements, false otherwise.
    */
    bool isFollow(Wildcard wildcard, Wildcard wildcard2);

    /**
    * @brief Retrieves all statements that follow a wildcard pattern and match a specified statement type.
    *
    * This method returns an unordered set of statement numbers that follow the specified wildcard pattern (wildcard)
    * and match the specified statement type (entity).
    *
    * @param wildcard A wildcard pattern for preceding statements.
    * @param entity The type of statement for which to search for following statements.
    * @return An unordered set of statement numbers that follow the wildcard pattern and match the type.
    */
    std::unordered_set<statementNumber> followStar(Wildcard wildcard, StmtEntity entity);

    /**
    * @brief Retrieves all statements that are followed by a specific statement number and match a specified statement type.
    *
    * This method returns an unordered set of statement numbers that are followed by the specified statement number (num)
    * and match the specified statement type (entity).
    *
    * @param num The statement number for which to find following statements.
    * @param entity The type of statement for which to search for following statements.
    * @return An unordered set of statement numbers that are followed by the specified statement number and match the type.
    */
    std::unordered_set<statementNumber> followStar(statementNumber num, StmtEntity entity);

    /**
    * @brief Retrieves all statements that are followed by statements matching a specified type and a specific statement number.
    *
    * This method returns an unordered set of statement numbers that are followed by statements matching the specified statement type (entity)
    * and the given statement number (num).
    *
    * @param entity The type of statement for which to search for preceding statements.
    * @param num The statement number for which to find preceding statements.
    * @return An unordered set of statement numbers that are followed by matching statements of the specified type.
    */
    std::unordered_set<statementNumber> followStar(StmtEntity entity, statementNumber num);

    /**
    * @brief Retrieves all statements that are followed by statements matching a specified type and a wildcard pattern.
    *
    * This method returns an unordered set of statement numbers that are followed by statements matching the specified statement type (entity)
    * and the given wildcard pattern (wildcard).
    *
    * @param entity The type of statement for which to search for preceding statements.
    * @param wildcard A wildcard pattern for following statements.
    * @return An unordered set of statement numbers that are followed by matching statements of the specified type.
    */
    std::unordered_set<statementNumber> followStar(StmtEntity entity, Wildcard wildcard);

    /**
    * @brief Retrieves pairs of preceding and following statement numbers based on statement types.
    *
    * This method returns an unordered set of pairs, where each pair consists of a preceding statement number and a following statement number,
    * for specified statement types (entity1 and entity2) representing follow-star relationships.
    *
    * @param entity1 The type of statement for which to retrieve preceding statements.
    * @param entity2 The type of statement for which to retrieve following statements.
    * @return An unordered set of pairs representing follow-star relationships for the specified statement types.
    */
    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
        followStar(StmtEntity entity1, StmtEntity entity2);

    /**
    * @brief Checks if one statement follows another statement in the follow-star relationship.
    *
    * This method checks if the statement specified by 'statement1' follows the statement specified by 'statement2'
    * in the follow-star relationship, considering all possible transitive following relationships.
    *
    * @param statement1 The statement number to check as the following statement.
    * @param statement2 The statement number to check as the preceding statement.
    * @return true if 'statement1' follows 'statement2' in the follow-star relationship, false otherwise.
    */
    bool isFollowStar(statementNumber statement1, statementNumber statement2);

    /**
    * @brief Checks if a statement, specified by its number, follows statements matching a wildcard pattern and a given type
    * in the follow-star relationship.
    *
    * This method checks if the statement specified by 'statement1' follows statements matching a wildcard pattern (wildcard)
    * and the specified statement type (entity) in the follow-star relationship, considering all possible transitive following relationships.
    *
    * @param statement1 The statement number to check as the following statement.
    * @param wildcard A wildcard pattern for preceding statements.
    * @return true if 'statement1' follows matching preceding statements in the follow-star relationship, false otherwise.
    */
    bool isFollowStar(statementNumber statement1, Wildcard wildcard);

    /**
    * @brief Checks if statements matching a wildcard pattern are followed by a statement specified by its number
    * in the follow-star relationship.
    *
    * This method checks if statements matching the wildcard pattern (wildcard) are followed by the statement specified by 'statement1'
    * in the follow-star relationship, considering all possible transitive following relationships.
    *
    * @param wildcard A wildcard pattern for following statements.
    * @param statement1 The statement number to check as the following statement.
    * @return true if matching preceding statements are followed by 'statement1' in the follow-star relationship, false otherwise.
    */
    bool isFollowStar(Wildcard wildcard, statementNumber statement1);

    /**
    * @brief Checks if statements matching a wildcard pattern are followed by statements matching another wildcard pattern
    * in the follow-star relationship.
    *
    * This method checks if statements matching the first wildcard pattern (wildcard) are followed by statements matching
    * the second wildcard pattern (wildcard2) in the follow-star relationship, considering all possible transitive following relationships.
    *
    * @param wildcard A wildcard pattern for preceding statements.
    * @param wildcard2 A wildcard pattern for following statements.
    * @return true if matching preceding statements are followed by matching following statements in the follow-star relationship, false otherwise.
    */
    bool isFollowStar(Wildcard wildcard, Wildcard wildcard2);

    /**
    * @brief Stores a mapping of statement numbers to sets of variables for IF statements.
    *
    * This function allows you to store a mapping of statement numbers to sets of variables
    * for IF statements in the provided variableMap.
    *
    * @param variableMap A std::unordered_map where statementNumber is the key,
    *                    and std::unordered_set<variable> is the associated value.
    */
    void storeIf(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap);

    /**
    * @brief Retrieves a set of statement numbers associated with IF statements matching the given Wildcard.
    *
    * This function returns a set of statement numbers corresponding to IF statements that match the provided Wildcard.
    *
    * @param wc The Wildcard to match IF statements against.
    * @return A set of statement numbers corresponding to IF statements matching the Wildcard.
    */
    std::unordered_set<statementNumber> getIf(Wildcard wc);

    /**
    * @brief Retrieves a set of statement numbers associated with IF statements containing the specified variable.
    *
    * This function returns a set of statement numbers corresponding to IF statements that contain the provided variable.
    *
    * @param v The variable to match against IF statements.
    * @return A set of statement numbers corresponding to IF statements containing the variable.
    */
    std::unordered_set<statementNumber> getIf(variable v);

    /**
    * @brief Retrieves all IF statements, represented as pairs of statementNumber and variable, from the data.
    *
    * This function retrieves all IF statements from the data and returns them as pairs of
    * statementNumber and variable.
    *
    * @return A set of pairs, where each pair represents an IF statement (statementNumber, variable).
    */
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAllIf();

    /**
    * @brief Stores a mapping of statement numbers to sets of variables for WHILE statements.
    *
    * This function allows you to store a mapping of statement numbers to sets of variables
    * for WHILE statements in the provided variableMap.
    *
    * @param variableMap A std::unordered_map where statementNumber is the key,
    *                    and std::unordered_set<variable> is the associated value.
    */
    void storeWhile(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap);

    /**
    * @brief Retrieves a set of statement numbers associated with WHILE statements matching the given Wildcard.
    *
    * This function returns a set of statement numbers corresponding to WHILE statements that match the provided Wildcard.
    *
    * @param wc The Wildcard to match WHILE statements against.
    * @return A set of statement numbers corresponding to WHILE statements matching the Wildcard.
    */
    std::unordered_set<statementNumber> getWhile(Wildcard wc);

    /**
    * @brief Retrieves a set of statement numbers associated with WHILE statements containing the specified variable.
    *
    * This function returns a set of statement numbers corresponding to WHILE statements that contain the provided variable.
    *
    * @param v The variable to match against WHILE statements.
    * @return A set of statement numbers corresponding to WHILE statements containing the variable.
    */
    std::unordered_set<statementNumber> getWhile(variable v);

    /**
    * @brief Retrieves all WHILE statements, represented as pairs of statementNumber and variable, from the data.
    *
    * This function retrieves all WHILE statements from the data and returns them as pairs of
    * statementNumber and variable.
    *
    * @return A set of pairs, where each pair represents a WHILE statement (statementNumber, variable).
    */
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAllWhile();


    // CallStore methods

    /**
    * @brief Stores a call relationship table.
    *
    * This function stores a call relationship table, mapping procedures to the set of procedures they call.
    *
    * @param callTable An unordered_map where the key is a procedure, and the value is a set of
    *                  procedures called by the key procedure.
    */
    void storeCalls(std::unordered_map<procedure, std::unordered_set<procedure>> callTable);

    /**
    * @brief Retrieves the Call* (transitive call) relationship table.
    *
    * This function returns a map representing the transitive call relationship among procedures.
    *
    * @return An unordered_map where the key is a procedure, and the value is a set of procedures
    *         called directly or indirectly by the key procedure.
    */
    std::unordered_map<procedure, std::unordered_set<procedure>> getCallStar();

    /**
    * @brief Retrieves procedures that call a procedure matching a wildcard.
    *
    * This function returns procedures that call a procedure matching the provided wildcard.
    *
    * @param procedure The procedure to match against wildcard.
    * @param wildcard The wildcard used to match against calling procedures.
    * @return A set of procedures calling procedures matching the wildcard.
    */
    std::unordered_set<procedure> call(StmtEntity procedure, Wildcard wildcard);

    /**
    * @brief Retrieves procedures that match a wildcard and call a specific procedure.
    *
    * This function returns procedures matching a wildcard that call the provided procedure.
    *
    * @param wildcard The wildcard used to match against called procedures.
    * @param procedure The procedure to check for usage by procedures matching the wildcard.
    * @return A set of procedures called by the provided procedure and matching the wildcard.
    */
    std::unordered_set<procedure> call(Wildcard wildcard, StmtEntity procedure);

    /**
    * @brief Retrieves procedures that call a specific procedure.
    *
    * This function returns procedures that call the provided specific procedure.
    *
    * @param procedure The procedure being called.
    * @param proc The calling procedure.
    * @return A set of procedures calling the specified procedure.
    */
    std::unordered_set<procedure> call(procedure procedure, StmtEntity proc);

    /**
    * @brief Retrieves procedures matching a wildcard that call a specific procedure.
    *
    * This function returns procedures matching a wildcard that call the provided specific procedure.
    *
    * @param proc The calling procedure.
    * @param procedure The procedure being called.
    * @return A set of procedures matching the wildcard that call the specified procedure.
    */
    std::unordered_set<procedure> call(StmtEntity proc, procedure procedure);

    /**
    * @brief Checks if a specific procedure is called by procedures matching a wildcard.
    *
    * This function checks if the specified procedure is called by procedures matching the provided wildcard.
    *
    * @param procedure The procedure being called.
    * @param wildcard The wildcard used to match calling procedures.
    * @return True if the specified procedure is called by procedures matching the wildcard; false otherwise.
    */
    bool isCall(procedure procedure, Wildcard wildcard);

    /**
    * @brief Checks if procedures matching a wildcard call a specific procedure.
    *
    * This function checks if procedures matching the provided wildcard call the specified procedure.
    *
    * @param wildcard The wildcard used to match calling procedures.
    * @param procedure The procedure being called.
    * @return True if procedures matching the wildcard call the specified procedure; false otherwise.
    */
    bool isCall(Wildcard wildcard, procedure procedure);

    /**
    * @brief Checks if a specific procedure calls another procedure by its name.
    *
    * This function checks if the specified procedure calls another procedure by its name.
    *
    * @param procedure The calling procedure.
    * @param procedure2 The name of the called procedure.
    * @return True if the calling procedure calls the specified procedure by name; false otherwise.
    */
    bool isCall(procedure procedure, std::string procedure2);

    /**
    * @brief Checks if there is a Call relationship between two procedures matching wildcards.
    *
    * This function checks if there is a Call relationship between two procedures that match
    * the provided wildcards.
    *
    * @param procedure The calling procedure.
    * @param wildcard2 The wildcard for the called procedure.
    * @return True if a Call relationship exists; false otherwise.
    */
    bool isCall(Wildcard wildcard, Wildcard wildcard2);


    /**
    * @brief Retrieves all pairs of procedures representing the Call* (transitive call) relationship between two procedures.
    *
    * This function returns all pairs of procedures representing the transitive Call* relationship between two specified procedures.
    *
    * @param procedure1 The calling procedure.
    * @param procedure2 The called procedure.
    * @return A set of pairs, where each pair represents a procedure that directly or indirectly
    *         calls another procedure (procedure1, procedure2).
    */
    std::unordered_set<std::pair<procedure, procedure>, PairHash> call(StmtEntity procedure1, StmtEntity procedure2);

    /**
    * @brief Retrieves procedures that have a Call* (transitive call) relationship with a procedure matching a wildcard.
    *
    * This function returns procedures that have a transitive Call* relationship with a procedure matching the provided wildcard.
    *
    * @param procedure The procedure to match against wildcard.
    * @param wildcard The wildcard used to match against procedures with transitive call relationships.
    * @return A set of procedures that have a Call* relationship with a procedure matching the wildcard.
    */
    std::unordered_set<procedure> callStar(StmtEntity procedure, Wildcard wildcard);

    /**
    * @brief Retrieves procedures that match a wildcard and have a Call* (transitive call) relationship with a specific procedure.
    *
    * This function returns procedures matching a wildcard that have a transitive Call* relationship with the provided procedure.
    *
    * @param wildcard The wildcard used to match against procedures with transitive call relationships.
    * @param procedure The procedure with which procedures are checked for Call* relationships.
    * @return A set of procedures matching the wildcard that have a Call* relationship with the provided procedure.
    */
    std::unordered_set<procedure> callStar(Wildcard wildcard, StmtEntity procedure);

    /**
    * @brief Retrieves procedures that have a Call* (transitive call) relationship with a specific procedure.
    *
    * This function returns procedures that have a transitive Call* relationship with the provided specific procedure.
    *
    * @param procedure The specific procedure being called.
    * @param proc The calling procedure.
    * @return A set of procedures having a Call* relationship with the specified procedure.
    */
    std::unordered_set<procedure> callStar(procedure procedure, StmtEntity proc);

    /**
    * @brief Retrieves procedures matching a wildcard that have a Call* (transitive call) relationship with a specific procedure.
    *
    * This function returns procedures matching a wildcard that have a transitive Call* relationship with the provided specific procedure.
    *
    * @param prc The specific procedure being called.
    * @param procedure The calling procedure.
    * @return A set of procedures matching the wildcard having a Call* relationship with the specified procedure.
    */
    std::unordered_set<procedure> callStar(StmtEntity prc, procedure procedure);

    /**
    * @brief Checks if a specific procedure has a Call* (transitive call) relationship with procedures matching a wildcard.
    *
    * This function checks if the specified procedure has a transitive Call* relationship with procedures matching the provided wildcard.
    *
    * @param procedure The specific procedure being called.
    * @param wildcard The wildcard used to match calling procedures.
    * @return True if the specified procedure has a Call* relationship with procedures matching the wildcard; false otherwise.
    */
    bool isCallStar(procedure procedure, Wildcard wildcard);

    /**
    * @brief Checks if procedures matching a wildcard have a Call* (transitive call) relationship with a specific procedure.
    *
    * This function checks if procedures matching the provided wildcard have a transitive Call* relationship with the specified procedure.
    *
    * @param wildcard The wildcard used to match calling procedures.
    * @param procedure The specific procedure being called.
    * @return True if procedures matching the wildcard have a Call* relationship with the specified procedure; false otherwise.
    */
    bool isCallStar(Wildcard wildcard, procedure procedure);

    /**
    * @brief Checks if a specific procedure has a Call* (transitive call) relationship with a procedure identified by its name.
    *
    * This function checks if the specified procedure has a transitive Call* relationship with a procedure identified by its name.
    *
    * @param procedure The specific procedure being called.
    * @param procedure2 The name of the called procedure.
    * @return True if the specified procedure has a Call* relationship with the procedure identified by its name; false otherwise.
    */
    bool isCallStar(procedure procedure, std::string procedure2);

    /**
    * @brief Checks if there is a Call* (transitive call) relationship between two procedures matching wildcards.
    *
    * This function checks if there is a transitive Call* relationship between two procedures that match the provided wildcards.
    *
    * @param wildcard1 The wildcard for the calling procedure.
    * @param wildcard2 The wildcard for the called procedure.
    * @return True if a transitive Call* relationship exists; false otherwise.
    */
    bool isCallStar(Wildcard wildcard1, Wildcard wildcard2);


    /**
    * @brief Retrieves all pairs of procedures representing the Call* (transitive call) relationship between two procedures.
    *
    * This function returns all pairs of procedures representing the transitive Call* relationship between two specified procedures.
    *
    * @param procedure1 The calling procedure.
    * @param procedure2 The called procedure.
    * @return A set of pairs, where each pair represents a procedure that directly or indirectly
    *         calls another procedure (procedure1, procedure2).
    */
    std::unordered_set<std::pair<procedure, procedure>, PairHash>
    callStar(StmtEntity procedure1, StmtEntity procedure2);


    // Next Methods
    /**
    * @brief Stores a mapping of statement numbers to sets of statement numbers representing the Next relationship.
    *
    * This function allows you to store a mapping of statement numbers to sets of statement numbers
    * representing the Next relationship in the provided NextMap.
    *
    * @param NextMap An unordered_map where statementNumber is the key, and unordered_set<statementNumber> is the associated value.
    */
    void storeNext(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap);

    /**
    * @brief Stores a Control Flow Graph (CFG).
    *
    * This function stores a Control Flow Graph (CFG) representing the program's control flow.
    *
    * @param cfg The Control Flow Graph to be stored.
    */
    void storeCfg(Cfg cfg);

    /**
    * @brief Stores a mapping of statement numbers to corresponding CFG nodes in a CFG legend.
    *
    * This function allows you to store a mapping of statement numbers to shared pointers to CFG nodes
    * in the provided cfgLegend.
    *
    * @param cfgLegend An unordered_map where statementNumber is the key, and shared_ptr<CfgNode> is the associated value.
    */
    void storeCfgLegend(std::unordered_map<statementNumber, std::shared_ptr<CfgNode>> cfgLegend);

    /**
    * @brief Retrieves pairs of statement numbers representing the Next relationship between two specified statements.
    *
    * This function returns pairs of statement numbers representing the Next relationship between two specified statements.
    *
    * @param ent1 The first statement entity.
    * @param ent2 The second statement entity.
    * @return A set of pairs, where each pair represents a statement number that directly follows another statement (ent1, ent2).
    */
    std::set<std::pair<statementNumber, statementNumber>> Next(StmtEntity ent1, StmtEntity ent2);

    /**
    * @brief Retrieves statement numbers that immediately follow a statement matching the specified entity.
    *
    * This function returns statement numbers that immediately follow a statement matching the specified entity.
    *
    * @param ent The statement entity to match against.
    * @param wildcard A Wildcard representing statements that immediately follow the matched entity.
    * @return A set of statement numbers that immediately follow the specified entity.
    */
    std::set<statementNumber> Next(StmtEntity ent, Wildcard wildcard);

    /**
    * @brief Retrieves statement numbers that immediately follow a statement matching the specified entity.
    *
    * This function returns statement numbers that immediately follow a statement matching the specified entity.
    *
    * @param ent The statement entity to match against.
    * @param num The statement number to match as the predecessor.
    * @return A set of statement numbers that immediately follow the specified entity and match the specified predecessor.
    */
    std::set<statementNumber> Next(StmtEntity ent, statementNumber num);

    /**
    * @brief Retrieves statement numbers that immediately follow a statement matching a specified entity.
    *
    * This function returns statement numbers that immediately follow a statement matching a specified entity.
    *
    * @param wildcard A Wildcard representing statements to match as predecessors.
    * @param ent The statement entity to match against.
    * @return A set of statement numbers that immediately follow statements matching the specified entity.
    */
    std::set<statementNumber> Next(Wildcard wildcard, StmtEntity ent);

    /**
    * @brief Retrieves statement numbers that immediately follow a specified statement entity.
    *
    * This function returns statement numbers that immediately follow a specified statement entity.
    *
    * @param num The statement number to match as the predecessor.
    * @param ent The statement entity to match against.
    * @return A set of statement numbers that immediately follow the specified entity and match the specified predecessor.
    */
    std::set<statementNumber> Next(statementNumber num, StmtEntity ent);


    /**
    * @brief Checks if there is a Next* (transitive next) relationship between two statements matching Wildcards.
    *
    * This function checks if there is a Next* (transitive next) relationship between two statements that match the provided Wildcards.
    *
    * @param wildcard1 The Wildcard for the first statement.
    * @param wildcard2 The Wildcard for the second statement.
    * @return True if a Next* relationship exists; false otherwise.
    */
    bool isNext(Wildcard wildcard1, Wildcard wildcard2);

    /**
    * @brief Checks if there is a Next* (transitive next) relationship between a statement matching a Wildcard and a specific statement.
    *
    * This function checks if there is a Next* (transitive next) relationship between a statement matching a Wildcard and a specific statement.
    *
    * @param wildcard The Wildcard for the first statement.
    * @param num The specific statement number for the second statement.
    * @return True if a Next* relationship exists; false otherwise.
    */
    bool isNext(Wildcard wildcard, statementNumber num);

    /**
    * @brief Checks if there is a Next* (transitive next) relationship between a specific statement and a statement matching a Wildcard.
    *
    * This function checks if there is a Next* (transitive next) relationship between a specific statement and a statement matching a Wildcard.
    *
    * @param num The specific statement number for the first statement.
    * @param wildcard The Wildcard for the second statement.
    * @return True if a Next* relationship exists; false otherwise.
    */
    bool isNext(statementNumber num, Wildcard wildcard);

    /**
    * @brief Checks if there is a Next* (transitive next) relationship between two specific statements.
    *
    * This function checks if there is a Next* (transitive next) relationship between two specific statements.
    *
    * @param num1 The specific statement number for the first statement.
    * @param num2 The specific statement number for the second statement.
    * @return True if a Next* relationship exists; false otherwise.
    */
    bool isNext(statementNumber num1, statementNumber num2);

    /**
    * @brief Retrieves pairs of statement numbers representing the Next* (transitive next) relationship between two specified statements.
    *
    * This function returns pairs of statement numbers representing the Next* relationship between two specified statements.
    *
    * @param ent1 The first statement entity.
    * @param ent2 The second statement entity.
    * @return A set of pairs, where each pair represents a statement number that directly or indirectly follows another statement (ent1, ent2).
    */
    std::set<std::pair<statementNumber, statementNumber>> NextStar(StmtEntity ent1, StmtEntity ent2);

    /**
    * @brief Retrieves statement numbers that follow a statement matching the specified entity through a Next* (transitive next) relationship.
    *
    * This function returns statement numbers that follow a statement matching the specified entity through a Next* relationship.
    *
    * @param ent The statement entity to match against.
    * @param wildcard A Wildcard representing statements that follow the matched entity through Next* relationship.
    * @return A set of statement numbers that follow the specified entity through a Next* relationship.
    */
    std::set<statementNumber> NextStar(StmtEntity ent, Wildcard wildcard);

    /**
    * @brief Retrieves statement numbers that follow a statement matching the specified entity through a Next* (transitive next) relationship.
    *
    * This function returns statement numbers that follow a statement matching the specified entity through a Next* relationship.
    *
    * @param ent The statement entity to match against.
    * @param num The statement number to match as the predecessor.
    * @return A set of statement numbers that follow the specified entity through a Next* relationship and match the specified predecessor.
    */
    std::set<statementNumber> NextStar(StmtEntity ent, statementNumber num);

    /**
    * @brief Retrieves statement numbers that follow a statement matching a Wildcard through a Next* (transitive next) relationship.
    *
    * This function returns statement numbers that follow a statement matching a Wildcard through a Next* relationship.
    *
    * @param wildcard A Wildcard representing statements to match as predecessors.
    * @param ent The statement entity to match against.
    * @return A set of statement numbers that follow statements matching the specified entity through a Next* relationship.
    */
    std::set<statementNumber> NextStar(Wildcard wildcard, StmtEntity ent);

    /**
    * @brief Retrieves statement numbers that follow a specified statement entity through a Next* (transitive next) relationship.
    *
    * This function returns statement numbers that follow a specified statement entity through a Next* relationship.
    *
    * @param num The statement number to match as the predecessor.
    * @param ent The statement entity to match against.
    * @return A set of statement numbers that follow the specified entity through a Next* relationship and match the specified predecessor.
    */
    std::set<statementNumber> NextStar(statementNumber num, StmtEntity ent);

    /**
    * @brief Checks if there is a Next* (transitive next) relationship between two statements matching Wildcards.
    *
    * This function checks if there is a Next* (transitive next) relationship between two statements that match the provided Wildcards.
    *
    * @param wildcard1 The Wildcard for the first statement.
    * @param wildcard2 The Wildcard for the second statement.
    * @return True if a Next* relationship exists; false otherwise.
    */
    bool isNextStar(Wildcard wildcard1, Wildcard wildcard2);

    /**
    * @brief Checks if there is a Next* (transitive next) relationship between a statement matching a Wildcard and a specific statement.
    *
    * This function checks if there is a Next* (transitive next) relationship between a statement matching a Wildcard and a specific statement.
    *
    * @param wildcard The Wildcard for the first statement.
    * @param num The specific statement number for the second statement.
    * @return True if a Next* relationship exists; false otherwise.
    */
    bool isNextStar(Wildcard wildcard, statementNumber num);

    /**
    * @brief Checks if there is a Next* (transitive next) relationship between a specific statement and a statement matching a Wildcard.
    *
    * This function checks if there is a Next* (transitive next) relationship between a specific statement and a statement matching a Wildcard.
    *
    * @param num The specific statement number for the first statement.
    * @param wildcard The Wildcard for the second statement.
    * @return True if a Next* relationship exists; false otherwise.
    */
    bool isNextStar(statementNumber num, Wildcard wildcard);

    /**
    * @brief Checks if there is a Next* (transitive next) relationship between two specific statements.
    *
    * This function checks if there is a Next* (transitive next) relationship between two specific statements.
    *
    * @param num1 The specific statement number for the first statement.
    * @param num2 The specific statement number for the second statement.
    * @return True if a Next* relationship exists; false otherwise.
    */
    bool isNextStar(statementNumber num1, statementNumber num2);

    /**
    * @brief Clears the cache used for storing Next* (transitive next) relationships.
    *
    * This function clears the cache used for storing Next* relationships, allowing for recalculation when needed.
    */
    void clearNextStarCache();


    PKB(const PKB&) = delete;
    PKB& operator=(const PKB&) = delete;
};
