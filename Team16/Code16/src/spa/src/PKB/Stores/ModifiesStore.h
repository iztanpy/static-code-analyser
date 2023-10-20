#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "utils/hash_utils.h"

class ModifiesStore {
 private:
    typedef std::string variable;
    typedef std::string procedure;

    typedef int statementNumber;
    std::unordered_map<statementNumber, std::unordered_set<variable>> ModifiesVariableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> ModifiesVariableMapReverse;

    std::unordered_map<procedure, std::unordered_set<variable>> ModifiesProcedureMap;
    std::unordered_map<variable, std::unordered_set<procedure>> ModifiesProcedureMapReverse;


 public:
    ModifiesStore();

    /**
    * @brief Stores modifies relationships between statements and variables.
    *
    * This method stores modifies relationships represented by an unordered map (relations), where each statement number (key)
    * is associated with a set of variables (values) that are modified by that statement.
    *
    * @param relations An unordered map of statement numbers to sets of variables modified by each statement.
    */
    void storeModifies(std::unordered_map<statementNumber, std::unordered_set<variable>> relations);


    void storeModifiesProcedures(std::unordered_map<procedure,
        std::pair<int, int>> procedures, std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar);

    void storeModifiesCalls(std::unordered_map<statementNumber, procedure> calls);

    /**
    * @brief Checks if a specific statement modifies a particular variable.
    *
    * This method checks if the statement specified by 'statement' modifies the variable specified by 'variable'.
    *
    * @param statement The statement number to check for variable modification.
    * @param variable The variable to check for modification by the statement.
    * @return true if the statement modifies the variable, false otherwise.
    */
    bool isModifies(statementNumber statement, variable variable);

    /**
    * @brief Checks if a specific statement modifies any variable.
    *
    * This method checks if the statement specified by 'statement' modifies any variables.
    *
    * @param statement The statement number to check for variable modification.
    * @return true if the statement modifies at least one variable, false otherwise.
    */
    bool isModifies(statementNumber statement);

    /**
    * @brief Retrieves the variables modified by a specific statement.
    *
    * This method returns an unordered set containing the variables that are modified by the statement specified by 'statement'.
    *
    * @param statement The statement number for which to retrieve modified variables.
    * @return An unordered set of variables modified by the specified statement.
    */
    std::unordered_set<variable> modifies(statementNumber statement);

    /**
    * @brief Checks if a given procedure has any 'Modifies' relationships.
    *
    * This function checks whether the specified procedure has any 'Modifies' relationships,
    * indicating whether it modifies any variables.
    *
    * @param procedure The procedure to check for 'Modifies' relationships.
    * @return True if the procedure has 'Modifies' relationships; false otherwise.
    */
    bool isModifies(procedure procedure);

    /**
    * @brief Retrieves a set of variables modified by a specific procedure.
    *
    * This function returns a set of variables that are modified by the provided procedure.
    *
    * @param procedure The procedure for which to retrieve modified variables.
    * @return A set of variables modified by the specified procedure.
    */
    std::unordered_set<variable> modifiesProcedureProc(procedure procedure);

    /**
    * @brief Checks if a given procedure modifies a specific variable.
    *
    * This function checks whether the specified procedure modifies the provided variable.
    *
    * @param procedure The procedure to check for 'Modifies' relationships.
    * @param variable The variable to check for modification by the procedure.
    * @return True if the procedure modifies the specified variable; false otherwise.
    */
    bool isModifies(procedure procedure, variable variable);

    /**
    * @brief Retrieves procedures that have 'Modifies' relationships, indicating they modify variables.
    *
    * This function returns a set of procedures that have 'Modifies' relationships, indicating they modify variables.
    *
    * @return A set of procedures that have 'Modifies' relationships.
    */
    std::unordered_set<procedure> modifiesProcedure();

    /**
    * @brief Retrieves procedures that modify a specific variable.
    *
    * This function returns a set of procedures that modify the provided variable.
    *
    * @param variable The variable to check for modification by procedures.
    * @return A set of procedures that modify the specified variable.
    */
    std::unordered_set<procedure> modifiesProcedure(variable variable);

    /**
    * @brief Retrieves all pairs of procedures and variables indicating 'Modifies' relationships.
    *
    * This function returns all pairs of procedures and variables that indicate 'Modifies' relationships.
    *
    * @return A set of pairs, where each pair represents a procedure and a variable (procedure, variable)
    *         indicating the procedure modifies the variable.
    */
    std::unordered_set<std::pair<procedure, variable>, PairHash> modifiesProcedurePair();


    /**
    * @brief Retrieves the statements that modify a particular variable.
    *
    * This method returns an unordered set of statement numbers representing statements that modify the variable specified by 'variable'.
    *
    * @param variable The variable for which to find modifying statements.
    * @return An unordered set of statement numbers of statements that modify the specified variable.
    */
    std::unordered_set<statementNumber> modifies(variable variable);
};
