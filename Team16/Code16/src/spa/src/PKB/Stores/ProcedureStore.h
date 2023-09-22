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

    void addProcedures(std::set<procedure> procedures);

    std::unordered_set<procedure> getAllProcedures();
};
