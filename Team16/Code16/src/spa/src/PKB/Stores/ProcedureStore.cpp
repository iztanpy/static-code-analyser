#include "ProcedureStore.h"


ProcedureStore::ProcedureStore() {
    this->procedureSet = std::unordered_set<procedure>();
}

void ProcedureStore::addProcedures(std::set<procedure> procedures) {
    this->procedureSet.insert(procedures.begin(), procedures.end());
}

std::unordered_set<ProcedureStore::procedure> ProcedureStore::getAllProcedures() {
    return this->procedureSet;
}
