#include "PKB/Stores/procedure_store.h"

typedef std::string procedure;

procedure_store::procedure_store() {
  this->procedureSet = std::unordered_set<procedure>();
}

void procedure_store::addProcedures(std::set<procedure> procedures) {
  this->procedureSet.insert(procedures.begin(), procedures.end());
}

void procedure_store::addProcedures(std::unordered_map<procedure, std::pair<startStatement, endStatement>> procedures) {
  this->procedureMap = procedures;
}

std::unordered_set<procedure_store::procedure> procedure_store::getAllProcedures() {
  return this->procedureSet;
}

std::pair<procedure_store::startStatement, procedure_store::endStatement> procedure_store::getProcedureRange
    (procedure procedureName) {
  return this->procedureMap[procedureName];
}

procedure procedure_store::getProcedureName(startStatement statementNumber) {
  for (auto const& x : this->procedureMap) {
    if (x.second.first <= statementNumber && x.second.second >= statementNumber) {
      return x.first;
    }
  }
  return "";
}
