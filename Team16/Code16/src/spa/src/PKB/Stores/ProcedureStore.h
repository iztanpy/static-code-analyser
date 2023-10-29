#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <utility>

class ProcedureStore {
 private:
    typedef std::string procedure;
    typedef int startStatement;
    typedef int endStatement;

    std::unordered_set<procedure> procedureSet;
    std::unordered_map<procedure, std::pair<startStatement, endStatement>> procedureMap;

 public:
    ProcedureStore();

    void addProcedures(std::unordered_map<procedure, std::pair<startStatement, endStatement>> procedures);

    /**
    * @brief Adds procedures to the procedure store.
    *
    * This method adds procedures represented by a set (procedures) to the procedure store.
    *
    * @param procedures A set of procedures to be added to the procedure store.
    */
    void addProcedures(std::set<procedure> procedures);

    /**
    * @brief Retrieves all procedures stored in the procedure store.
    *
    * This method returns an unordered set containing all the procedures stored in the procedure store.
    *
    * @return An unordered set of all procedures stored in the procedure store.
    */
    std::unordered_set<procedure> getAllProcedures();

    std::pair<startStatement, endStatement> getProcedureRange(procedure procedureName);

    procedure getProcedureName(startStatement statementNumber);
};
