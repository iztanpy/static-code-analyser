#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "utils/hash_utils.h"
#include "utils/clauses_types.h"

class UsesStore {
 private:
    typedef std::string variable;
    typedef std::string constant;
    typedef int statementNumber;
    typedef std::string procedure;

    std::unordered_map<statementNumber, std::unordered_set<variable>> UsesVariableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> UsesVariableMapReverse;

    std::unordered_map<procedure, std::unordered_set<variable>> UsesProcedureMap;
    std::unordered_map<variable, std::unordered_set<procedure>> UsesProcedureMapReverse;

 public:
    UsesStore();

    /**
    * @brief Stores usage information for variables in statements.
    *
    * This method stores usage information, where each statement number (key) is associated with a set of variables (values)
    * that are used in that statement.
    *
    * @param statementUsesMap An unordered map of statement numbers to sets of variables used in each statement.
    */
    void storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> statementUsesMap);

    void storeUsesProcedures(std::unordered_map<procedure,
        std::pair<int, int>> procedures, std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar);

    /**
    * @brief Checks if a specific statement uses a particular variable.
    *
    * This method checks if the statement specified by 'lineNumber' uses the variable specified by 'variableName'.
    *
    * @param lineNumber The statement number to check for variable usage.
    * @param variableName The name of the variable to check for usage in the statement.
    * @return true if the statement uses the variable, false otherwise.
    */
    bool isUses(statementNumber lineNumber, variable variableName);

    /**
    * @brief Checks if a specific statement uses any variable.
    *
    * This method checks if the statement specified by 'lineNumber' uses any variables.
    *
    * @param lineNumber The statement number to check for variable usage.
    * @return true if the statement uses at least one variable, false otherwise.
    */
    bool isUses(statementNumber lineNumber);

    /**
    * @brief Retrieves the variables used in a specific statement.
    *
    * This method returns an unordered set containing the variables that are used in the statement specified by 'lineNumber'.
    *
    * @param lineNumber The statement number for which to retrieve used variables.
    * @return An unordered set of variables used in the specified statement.
    */
    std::unordered_set<variable> uses(statementNumber lineNumber);

    /**
    * @brief Retrieves the statements that use a particular variable.
    *
    * This method returns an unordered set of statement numbers representing statements that use the variable specified by 'variableName'.
    *
    * @param variableName The name of the variable for which to find using statements.
    * @return An unordered set of statement numbers of statements that use the specified variable.
    */
    std::unordered_set<statementNumber> uses(variable variableName);

    bool isUses(procedure procedure);

    std::unordered_set<variable> usesProcedureProc(procedure procedure);

    bool isUses(procedure procedure, variable variable);

    std::unordered_set<procedure> usesProcedure();

    std::unordered_set<procedure> usesProcedure(variable variable);

    std::unordered_set<std::pair<procedure, variable>, PairHash> usesProcedurePair();
};
