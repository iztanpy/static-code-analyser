#include <iostream>
#include <unordered_set>

#include "PKB/API/ReadFacade.h"

ReadFacade::ReadFacade(PKB& pkb) : pkb(pkb) {}

std::unordered_set<statementNumber> ReadFacade::getAllAssigns() {
  return pkb.getAllAssigns();
}

std::unordered_set<statementNumber> ReadFacade::getAssigns(variable LHS, possibleCombinations RHS) {
  return pkb.getAssigns(LHS, RHS);
}

std::unordered_set<statementNumber> ReadFacade::getAssigns(variable LHS, Wildcard RHS) {
  return pkb.getAssigns(LHS, RHS);
}

std::unordered_set<statementNumber> ReadFacade::getAssigns(Wildcard LHS, possibleCombinations RHS) {
  return pkb.getAssigns(LHS, RHS);
}

std::unordered_set<statementNumber> ReadFacade::getAssigns(Wildcard LHS, Wildcard RHS) {
  return pkb.getAssigns(LHS, RHS);
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> ReadFacade::getAssignPair(Wildcard wildcard) {
  return pkb.getAssignPair(wildcard);
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> ReadFacade::getAssignPair(partialMatch partial) {
  return pkb.getAssignPair(partial);
}

std::unordered_set<variable> ReadFacade::getVariables() {
  return pkb.getVariables();
}

std::unordered_set<procedure> ReadFacade::getProcedures() {
  return pkb.getProcedures();
}

bool ReadFacade::isUses(statementNumber lineNumber, variable variableName) {
  return pkb.isUses(lineNumber, variableName);
}

bool ReadFacade::isUses(statementNumber lineNumber, Wildcard wildcard) {
  return pkb.isUses(lineNumber, wildcard);
}

std::unordered_set<variable> ReadFacade::uses(statementNumber line) {
  return pkb.uses(line);
}

std::unordered_set<statementNumber> ReadFacade::uses(StmtEntity type, variable variableName) {
  return pkb.uses(type, variableName);
}

std::unordered_set<statementNumber> ReadFacade::uses(StmtEntity type, Wildcard wildcard) {
  return pkb.uses(type, wildcard);
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> ReadFacade::uses(StmtEntity type) {
  return pkb.uses(type);
}

bool ReadFacade::isUses(procedure procedure, Wildcard wildcard) {
    return pkb.isUses(procedure, wildcard);
}

std::unordered_set<variable> ReadFacade::uses(procedure procedure) {
    return pkb.uses(procedure);
}

bool ReadFacade::isUses(procedure procedure, variable variableName) {
    return pkb.isUses(procedure, variableName);
}

std::unordered_set<procedure> ReadFacade::usesProcedure(Wildcard wildcard) {
    return pkb.usesProcedure(wildcard);
}

std::unordered_set<procedure> ReadFacade::usesProcedure(variable variableName) {
    return pkb.usesProcedure(variableName);
}

std::unordered_set<std::pair<procedure, variable>, PairHash> ReadFacade::usesProcedure() {
    return pkb.usesProcedure();
}

bool ReadFacade::isModifies(statementNumber lineNumber, variable variableName) {
  return pkb.isModifies(lineNumber, variableName);
}

bool ReadFacade::isModifies(statementNumber lineNumber, Wildcard wildcard) {
  return pkb.isModifies(lineNumber, wildcard);
}

std::unordered_set<variable> ReadFacade::modifies(statementNumber line) {
  return pkb.modifies(line);
}

std::unordered_set<statementNumber> ReadFacade::modifies(StmtEntity type, variable variableName) {
  return pkb.modifies(type, variableName);
}

std::unordered_set<statementNumber> ReadFacade::modifies(StmtEntity type, Wildcard wildcard) {
  return pkb.modifies(type, wildcard);
}

bool ReadFacade::isModifies(procedure procedure, Wildcard wildcard) {
    return pkb.isModifies(procedure, wildcard);
}

std::unordered_set<variable> ReadFacade::modifies(procedure procedure) {
    return pkb.modifies(procedure);
}

bool ReadFacade::isModifies(procedure procedure, variable variableName) {
    return pkb.isModifies(procedure, variableName);
}

std::unordered_set<procedure> ReadFacade::modifiesProcedure(Wildcard wildcard) {
    return pkb.modifiesProcedure(wildcard);
}

std::unordered_set<procedure> ReadFacade::modifiesProcedure(variable variableName) {
    return pkb.modifiesProcedure(variableName);
}

std::unordered_set<std::pair<procedure, variable>, PairHash> ReadFacade::modifiesProcedure() {
    return pkb.modifiesProcedure();
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> ReadFacade::modifies(StmtEntity type) {
  return pkb.modifies(type);
}

std::unordered_set<constant> ReadFacade::getConstants() {
  return pkb.getConstants();
}

std::unordered_set<statementNumber> ReadFacade::getStatements(StmtEntity type) {
  return pkb.getStatements(type);
}

std::unordered_set<statementNumber> ReadFacade::parent(Wildcard wildcard, StmtEntity entity) {
  return pkb.parent(wildcard, entity);
}

std::unordered_set<statementNumber> ReadFacade::parent(statementNumber num, StmtEntity entity) {
  return pkb.parent(num, entity);
}

std::unordered_set<statementNumber> ReadFacade::parent(StmtEntity entity, statementNumber num) {
  return pkb.parent(entity, num);
}

std::unordered_set<statementNumber> ReadFacade::parent(StmtEntity entity, Wildcard wildcard) {
  return pkb.parent(entity, wildcard);
}

std::unordered_set<statementNumber> ReadFacade::parent(statementNumber statement, Wildcard wildcard) {
  return pkb.parent(statement, wildcard);
}

std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
ReadFacade::parent(StmtEntity entity, StmtEntity entity2) {
  return pkb.parent(entity, entity2);
}

bool ReadFacade::isParent(statementNumber parent, statementNumber child) {
  return pkb.isParent(parent, child);
}

bool ReadFacade::isParent(statementNumber parent, Wildcard wildcard) {
  return pkb.isParent(parent, wildcard);
}

bool ReadFacade::isParent(Wildcard wildcard, statementNumber child) {
  return pkb.isParent(wildcard, child);
}

bool ReadFacade::isParent(Wildcard wildcard, Wildcard wildcard2) {
  return pkb.isParent(wildcard, wildcard2);
}

std::unordered_set<statementNumber> ReadFacade::parentStar(Wildcard wildcard, StmtEntity entity) {
  return pkb.parentStar(wildcard, entity);
}

std::unordered_set<statementNumber> ReadFacade::parentStar(statementNumber num, StmtEntity entity) {
  return pkb.parentStar(num, entity);
}

std::unordered_set<statementNumber> ReadFacade::parentStar(StmtEntity entity, statementNumber num) {
  return pkb.parentStar(entity, num);
}

std::unordered_set<statementNumber> ReadFacade::parentStar(StmtEntity entity, Wildcard wildcard) {
  return pkb.parentStar(entity, wildcard);
}

std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
ReadFacade::parentStar(StmtEntity entity, StmtEntity entity2) {
  return pkb.parentStar(entity, entity2);
}

bool ReadFacade::isParentStar(statementNumber parent, statementNumber child) {
  return pkb.isParentStar(parent, child);
}

bool ReadFacade::isParentStar(statementNumber parent, Wildcard wildcard) {
  return pkb.isParentStar(parent, wildcard);
}

bool ReadFacade::isParentStar(Wildcard wildcard, statementNumber child) {
  return pkb.isParentStar(wildcard, child);
}

bool ReadFacade::isParentStar(Wildcard wildcard, Wildcard wildcard2) {
  return pkb.isParentStar(wildcard, wildcard2);
}

std::unordered_set<statementNumber> ReadFacade::follows(Wildcard wildcard, StmtEntity entity) {
  return pkb.follows(wildcard, entity);
}

std::unordered_set<statementNumber> ReadFacade::follows(statementNumber num, StmtEntity entity) {
  return pkb.follows(num, entity);
}

std::unordered_set<statementNumber> ReadFacade::follows(StmtEntity entity, statementNumber num) {
  return pkb.follows(entity, num);
}

std::unordered_set<statementNumber> ReadFacade::follows(StmtEntity entity, Wildcard wildcard) {
  return pkb.follows(entity, wildcard);
}

std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> ReadFacade::follows(StmtEntity entity1,
                                                                                              StmtEntity entity2) {
  return pkb.follows(entity1, entity2);
}

bool ReadFacade::isFollow(statementNumber statement1, statementNumber statement2) {
  return pkb.isFollow(statement1, statement2);
}

bool ReadFacade::isFollow(statementNumber statement1, Wildcard wildcard) {
  return pkb.isFollow(statement1, wildcard);
}

bool ReadFacade::isFollow(Wildcard wildcard, statementNumber statement1) {
  return pkb.isFollow(wildcard, statement1);
}

bool ReadFacade::isFollow(Wildcard wildcard, Wildcard wildcard2) {
  return pkb.isFollow(wildcard, wildcard2);
}

std::unordered_set<statementNumber> ReadFacade::followStar(Wildcard wildcard, StmtEntity entity) {
  return pkb.followStar(wildcard, entity);
}

std::unordered_set<statementNumber> ReadFacade::followStar(statementNumber num, StmtEntity entity) {
  return pkb.followStar(num, entity);
}

std::unordered_set<statementNumber> ReadFacade::followStar(StmtEntity entity, statementNumber num) {
  return pkb.followStar(entity, num);
}

std::unordered_set<statementNumber> ReadFacade::followStar(StmtEntity entity, Wildcard wildcard) {
  return pkb.followStar(entity, wildcard);
}

std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> ReadFacade::followStar(StmtEntity entity1,
                                                                                                 StmtEntity entity2) {
  return pkb.followStar(entity1, entity2);
}

bool ReadFacade::isFollowStar(statementNumber statement1, statementNumber statement2) {
  return pkb.isFollowStar(statement1, statement2);
}

bool ReadFacade::isFollowStar(statementNumber statement1, Wildcard wildcard) {
  return pkb.isFollowStar(statement1, wildcard);
}

bool ReadFacade::isFollowStar(Wildcard wildcard, statementNumber statement1) {
  return pkb.isFollowStar(wildcard, statement1);
}

bool ReadFacade::isFollowStar(Wildcard wildcard, Wildcard wildcard2) {
  return pkb.isFollowStar(wildcard, wildcard2);
}

std::unordered_set<procedure> ReadFacade::call(Dec declaration, Wildcard wildcard) {
    return pkb.call(declaration, wildcard);
}

std::unordered_set<procedure> ReadFacade::call(Wildcard wildcard, Dec declaration) {
    return pkb.call(wildcard, declaration);
}

std::unordered_set<procedure> ReadFacade::call(procedure procedure, Wildcard wildcard) {
    return pkb.call(procedure, wildcard);
}

std::unordered_set<procedure> ReadFacade::call(Wildcard wildcard, procedure procedure) {
    return pkb.call(wildcard, procedure);
}

bool ReadFacade::isCall(procedure procedure, Wildcard wildcard) {
    return pkb.isCall(procedure, wildcard);
}

bool ReadFacade::isCall(Wildcard wildcard, procedure procedure) {
    return pkb.isCall(wildcard, procedure);
}

bool ReadFacade::isCall(procedure procedure, std::string procedure2) {
    return pkb.isCall(procedure, procedure2);
}

bool ReadFacade::isCall(Wildcard wildcard1, Wildcard wildcard2) {
    return pkb.isCall(wildcard1, wildcard2);
}

std::unordered_set<std::pair<procedure, procedure>, PairHash> ReadFacade::call(Dec declaration1, Dec declaration2) {
    return pkb.call(declaration1, declaration2);
}

std::unordered_set<procedure> ReadFacade::callStar(Dec declaration, Wildcard wildcard) {
    return pkb.callStar(declaration, wildcard);
}

std::unordered_set<procedure> ReadFacade::callStar(Wildcard wildcard, Dec declaration) {
    return pkb.callStar(wildcard, declaration);
}

std::unordered_set<procedure> ReadFacade::callStar(procedure procedure, Wildcard wildcard) {
    return pkb.callStar(procedure, wildcard);
}

std::unordered_set<procedure> ReadFacade::callStar(Wildcard wildcard, procedure procedure) {
    return pkb.callStar(wildcard, procedure);
}

bool ReadFacade::isCallStar(procedure procedure, Wildcard wildcard) {
    return pkb.isCallStar(procedure, wildcard);
}

bool ReadFacade::isCallStar(Wildcard wildcard, procedure procedure) {
    return pkb.isCallStar(wildcard, procedure);
}

bool ReadFacade::isCallStar(procedure procedure, std::string procedure2) {
    return pkb.isCallStar(procedure, procedure2);
}

bool ReadFacade::isCallStar(Wildcard wildcard1, Wildcard wildcard2) {
    return pkb.isCallStar(wildcard1, wildcard2);
}

std::unordered_set<std::pair<procedure, procedure>, PairHash> ReadFacade::callStar(Dec declaration1, Dec declaration2) {
    return pkb.callStar(declaration1, declaration2);
}




