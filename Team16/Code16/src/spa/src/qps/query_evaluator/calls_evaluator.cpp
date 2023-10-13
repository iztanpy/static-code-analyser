#include "calls_evaluator.h"

Constraint CallsEvaluator::Handle(Declaration& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_CallT) {
  // If lhs and rhs are the same declaration, return false to signify empty
  // Only Affect/Next should have non-empty results
  if (lhs.equals(rhs)) {
    return false;
  }
  StmtEntity lhs_stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  StmtEntity rhs_stmt_entity = ConvertToStmtEntity(rhs.design_entity);
  if (is_CallT) {
    return BinaryConstraint{{lhs.synonym, rhs.synonym}, pkb_reader.callStar(lhs_stmt_entity, rhs_stmt_entity)};
  }
  return BinaryConstraint{{lhs.synonym, rhs.synonym}, pkb_reader.call(lhs_stmt_entity, rhs_stmt_entity)};
}

UnaryConstraint CallsEvaluator::Handle(Declaration& lhs, std::string& rhs, ReadFacade& pkb_reader, bool is_CallT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  if (is_CallT) {
    return {lhs.synonym, pkb_reader.callStar(stmt_entity, rhs)};
  }
  return {lhs.synonym, pkb_reader.call(stmt_entity, rhs)};
}

UnaryConstraint CallsEvaluator::Handle(Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_CallT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  if (is_CallT) {
    return {lhs.synonym, pkb_reader.callStar(stmt_entity, rhs)};
  }
  return {lhs.synonym, pkb_reader.call(stmt_entity, rhs)};
}

UnaryConstraint CallsEvaluator::Handle(std::string& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_CallT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(rhs.design_entity);
  if (is_CallT) {
    return {rhs.synonym, pkb_reader.callStar(lhs, stmt_entity)};
  }
  return {rhs.synonym, pkb_reader.call(lhs, stmt_entity)};
}

bool CallsEvaluator::Handle(std::string& lhs, std::string& rhs, ReadFacade& pkb_reader, bool is_CallT) {
  if (is_CallT) {
    return pkb_reader.isCallStar(lhs, rhs);
  }
  return pkb_reader.isCall(lhs, rhs);
}

bool CallsEvaluator::Handle(std::string& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_CallT) {
  if (is_CallT) {
    return pkb_reader.isCallStar(lhs, rhs);
  }
  return pkb_reader.isCall(lhs, rhs);
}

UnaryConstraint CallsEvaluator::Handle(Wildcard& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_CallT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(rhs.design_entity);
  if (is_CallT) {
    return {rhs.synonym, pkb_reader.callStar(lhs, stmt_entity)};
  }
  return {rhs.synonym, pkb_reader.call(lhs, stmt_entity)};
}

bool CallsEvaluator::Handle(Wildcard& lhs, std::string& rhs, ReadFacade& pkb_reader, bool is_CallT) {
  if (is_CallT) {
    return pkb_reader.isCallStar(lhs, rhs);
  }
  return pkb_reader.isCall(lhs, rhs);
}

bool CallsEvaluator::Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_CallT) {
  if (is_CallT) {
    return pkb_reader.isCallStar(lhs, rhs);
  }
  return pkb_reader.isCall(lhs, rhs);
}
