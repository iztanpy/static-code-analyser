#include "qps/query_evaluator/parent_evaluator.h"

bool ParentEvaluator::Handle(int lhs, int rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  if (is_ParentT) {
    return pkb_reader.isParentStar(lhs, rhs);
  }
  return pkb_reader.isParent(lhs, rhs);
}
UnaryConstraint ParentEvaluator::Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(rhs.design_entity);
  if (is_ParentT) {
    std::unordered_set<statementNumber> results = pkb_reader.parentStar(lhs, stmt_entity);
    return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
  }
  std::unordered_set<statementNumber> results = pkb_reader.parent(lhs, stmt_entity);
  return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
}
bool ParentEvaluator::Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  if (is_ParentT) {
    return pkb_reader.isParentStar(lhs, rhs);
  }
  return pkb_reader.isParent(lhs, rhs);
}
UnaryConstraint ParentEvaluator::Handle(Declaration& lhs, int rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  if (is_ParentT) {
    std::unordered_set<statementNumber> results = pkb_reader.parentStar(stmt_entity, rhs);
    return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
  }
  std::unordered_set<statementNumber> results = pkb_reader.parent(stmt_entity, rhs);
  return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
}
Constraint ParentEvaluator::Handle(Declaration& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  if (lhs.equals(rhs)) {
    return false;
  }
  StmtEntity lhs_stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  StmtEntity rhs_stmt_entity = ConvertToStmtEntity(rhs.design_entity);
  if (is_ParentT) {
    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> raw_results
        = pkb_reader.parentStar(lhs_stmt_entity, rhs_stmt_entity);
    return BinaryConstraint{{lhs.synonym, rhs.synonym}, EvaluatorUtil::ToStringPairSet(raw_results)};
  }
  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> raw_results
      = pkb_reader.parent(lhs_stmt_entity, rhs_stmt_entity);
  return BinaryConstraint{{lhs.synonym, rhs.synonym}, EvaluatorUtil::ToStringPairSet(raw_results)};
}
UnaryConstraint ParentEvaluator::Handle(Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  if (is_ParentT) {
    std::unordered_set<statementNumber> results = pkb_reader.parentStar(stmt_entity, rhs);
    return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
  }
  std::unordered_set<statementNumber> results = pkb_reader.parent(stmt_entity, rhs);
  return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
}
bool ParentEvaluator::Handle(Wildcard& lhs, int rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  if (is_ParentT) {
    return pkb_reader.isParentStar(lhs, rhs);
  }
  return pkb_reader.isParent(lhs, rhs);
}
UnaryConstraint ParentEvaluator::Handle(Wildcard& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(rhs.design_entity);
  if (is_ParentT) {
    std::unordered_set<statementNumber> results = pkb_reader.parentStar(lhs, stmt_entity);
    return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
  }
  std::unordered_set<statementNumber> results = pkb_reader.parent(lhs, stmt_entity);
  return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
}
bool ParentEvaluator::Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  if (is_ParentT) {
    return pkb_reader.isParentStar(lhs, rhs);
  }
  return pkb_reader.isParent(lhs, rhs);
}
