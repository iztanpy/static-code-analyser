#pragma once
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include "PKB/PKB.h"

typedef std::string variable;
typedef int statementNumber;
typedef std::string possibleCombinations;
typedef std::string constant;

class WriteFacade {
 private:
    PKB& pkb;

 public:
    explicit WriteFacade(PKB& pkb);

    /**
    * @brief Stores assignments information in the program.
    *
    * This method stores assignment information, including mappings of statement numbers to sets of possible combinations
    * (numRHSMap) and statement numbers to variables (numLHSMap).
    *
    * @param numRHSMap An unordered map of statement numbers to sets of possible combinations for the right-hand side of assignments.
    * @param numLHSMap An unordered map of statement numbers to variables for the left-hand side of assignments.
    */
    void storeAssignments(std::unordered_map<statementNumber, std::unordered_set<possibleCombinations>> numRHSMap,
                          std::unordered_map<statementNumber, variable> numLHSMap);

    void storeAssignments(std::unordered_map<statementNumber, std::unordered_set<partialMatch>> partialRHSMap,
                        std::unordered_map<statementNumber, full> fullRHSMap,
                        std::unordered_map<statementNumber, variable> numLHSMap);

    /**
    * @brief Stores variable information in the program.
    *
    * This method stores variable information by accepting a set of variables and adding them to the program's variable store.
    *
    * @param variables An unordered set of variables to be stored.
    */
    void storeVariables(std::unordered_set<variable> variables);

    /**
    * @brief Stores procedure information in the program.
    *
    * This method stores procedure information by accepting a set of procedure names and adding them to the program's procedure store.
    *
    * @param procedures A set of procedure names to be stored.
    */
    void storeProcedures(std::set<std::string> procedures);

    /**
    * @brief Stores uses information in the program.
    *
    * This method stores uses information, including mappings of statement numbers to sets of used variables (varUsesMap).
    *
    * @param varUsesMap An unordered map of statement numbers to sets of variables that are used.
    */
    void storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap);

    /**
    * @brief Stores modifies information in the program.
    *
    * This method stores modifies information, including mappings of statement numbers to modified variables (varModifiesMap).
    *
    * @param varModifiesMap An unordered map of statement numbers to variables that are modified.
    */
    void storeModifies(std::unordered_map<statementNumber, variable> varModifiesMap);

    /**
    * @brief Stores constant information in the program.
    *
    * This method stores constant information by accepting a set of constants and adding them to the program's constant store.
    *
    * @param constants An unordered set of constants to be stored.
    */
    void storeConstants(std::unordered_set<constant> constants);

    /**
    * @brief Stores statement information in the program.
    *
    * This method stores statement information, including mappings of statement numbers to statement entities (typeMap).
    *
    * @param typeMap An unordered map of statement numbers to statement entities.
    */
    void storeStatements(std::unordered_map<statementNumber, StmtEntity> typeMap);

    /**
    * @brief Stores parent relationship information in the program.
    *
    * This method stores parent relationship information, including mappings of statement numbers to sets of parent statements (map).
    *
    * @param map An unordered map of statement numbers to sets of parent statement numbers.
    */
    void storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map);

    /**
    * @brief Stores follows relationship information in the program.
    *
    * This method stores follows relationship information, including mappings of statement numbers to their follower statement numbers (map).
    *
    * @param map An unordered map of statement numbers to their follower statement numbers.
    */
    void storeFollows(std::unordered_map<statementNumber, statementNumber> map);
};
