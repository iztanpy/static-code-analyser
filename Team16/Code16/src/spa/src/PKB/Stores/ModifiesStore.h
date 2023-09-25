#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>

class ModifiesStore {
 private:
    typedef std::string variable;
    typedef int statementNumber;
    std::unordered_map<statementNumber, std::unordered_set<variable>> ModifiesVariableMap;
    std::unordered_map<variable, std::unordered_set<statementNumber>> ModifiesVariableMapReverse;

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
    * @brief Retrieves the statements that modify a particular variable.
    *
    * This method returns an unordered set of statement numbers representing statements that modify the variable specified by 'variable'.
    *
    * @param variable The variable for which to find modifying statements.
    * @return An unordered set of statement numbers of statements that modify the specified variable.
    */
    std::unordered_set<statementNumber> modifies(variable variable);
};
