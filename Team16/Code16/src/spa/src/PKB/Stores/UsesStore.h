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

    void storeUsesCalls(std::unordered_map<statementNumber, procedure> calls);

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

    /**
    * @brief Checks if a given procedure uses any variables.
    *
    * This function checks whether the specified procedure uses any variables.
    *
    * @param procedure The procedure to check for variable usage.
    * @return True if the procedure uses variables; false otherwise.
    */
    bool isUses(procedure procedure);

    /**
     * @brief Retrieves a set of variables used by a specific procedure.
     *
     * This function returns a set of variables used by the provided procedure.
     *
     * @param procedure The procedure for which to retrieve used variables.
     * @return A set of variables used by the specified procedure.
     */
    std::unordered_set<variable> usesProcedureProc(procedure procedure);

    /**
     * @brief Checks if a given procedure uses a specific variable.
     *
     * This function checks whether the specified procedure uses the provided variable.
     *
     * @param procedure The procedure to check for variable usage.
     * @param variable The variable to check for usage within the procedure.
     * @return True if the procedure uses the specified variable; false otherwise.
     */
    bool isUses(procedure procedure, variable variable);

    /**
     * @brief Retrieves a set of procedures that use variables.
     *
     * This function returns a set of procedures that use variables.
     *
     * @return A set of procedures that use variables.
     */
    std::unordered_set<procedure> usesProcedure();

    /**
     * @brief Retrieves a set of procedures that use a specific variable.
     *
     * This function returns a set of procedures that use the provided variable.
     *
     * @param variable The variable to check for usage in procedures.
     * @return A set of procedures that use the specified variable.
     */
    std::unordered_set<procedure> usesProcedure(variable variable);

    /**
     * @brief Retrieves all pairs of procedures and variables where the procedure uses the variable.
     *
     * This function returns all pairs of procedures and variables where the procedure uses the variable.
     *
     * @return A set of pairs, where each pair represents a procedure and a variable (procedure, variable).
     */
    std::unordered_set<std::pair<procedure, variable>, PairHash> usesProcedurePair();
};
