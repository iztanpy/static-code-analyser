#pragma once
#include <string>
#include <unordered_set>


class VariableStore {
 private:
    typedef std::string variable;
    std::unordered_set<variable> variables;
 public:
    VariableStore();

    /**
    * @brief Adds a set of variables to the variable store.
    *
    * This method adds a collection of variables represented by an unordered set (variables) to the variable store.
    *
    * @param variables An unordered set of variables to be added to the variable store.
    */
    void addVariables(std::unordered_set<variable> variables);

    /**
    * @brief Retrieves all variables stored in the variable store.
    *
    * This method returns an unordered set containing all the variables stored in the variable store.
    *
    * @return An unordered set of variables stored in the variable store.
    */
    std::unordered_set<variable> getVariables();
};
