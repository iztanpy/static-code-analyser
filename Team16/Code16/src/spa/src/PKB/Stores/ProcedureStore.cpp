#include "ProcedureStore.h"

typedef std::string procedure;

ProcedureStore::ProcedureStore() {
    this->procedureSet = std::unordered_set<procedure>();
}

void ProcedureStore::addProcedures(std::set<procedure> procedures) {
    this->procedureSet.insert(procedures.begin(), procedures.end());
}

void ProcedureStore::addProcedures(std::unordered_map<procedure, std::pair<startStatement, endStatement>> procedures) {
    this->procedureMap = procedures;
}

std::unordered_set<ProcedureStore::procedure> ProcedureStore::getAllProcedures() {
    return this->procedureSet;
}

std::pair<ProcedureStore::startStatement, ProcedureStore::endStatement> ProcedureStore::getProcedureRange
    (procedure procedureName) {
    return this->procedureMap[procedureName];
}

procedure ProcedureStore::getProcedureName(startStatement statementNumber) {
    for (auto const& x : this->procedureMap) {
        if (x.second.first <= statementNumber && x.second.second >= statementNumber) {
            return x.first;
        }
    }
    return "";
}