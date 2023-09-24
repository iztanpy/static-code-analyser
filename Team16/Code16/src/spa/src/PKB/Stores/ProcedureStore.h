#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>

class ProcedureStore {
 private:
    typedef std::string procedure;

    std::unordered_set<procedure> procedureSet;

 public:
    ProcedureStore();

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
};
