#include "qps/query_evaluator/follows_evaluator.h"

bool FollowsEvaluator::Handle(int lhs, int rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  if (is_FollowT) {
    return pkb_reader.isFollowStar(lhs, rhs);
  }
  return pkb_reader.isFollow(lhs, rhs);
}
UnaryConstraint FollowsEvaluator::Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(rhs.design_entity);
  if (is_FollowT) {
    std::unordered_set < statementNumber > results = pkb_reader.followStar(lhs, stmt_entity);
    return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
  }
  std::unordered_set < statementNumber > results = pkb_reader.follows(lhs, stmt_entity);
  return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
}
bool FollowsEvaluator::Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  if (is_FollowT) {
    return pkb_reader.isFollowStar(lhs, rhs);
  }
  return pkb_reader.isFollow(lhs, rhs);
}
UnaryConstraint FollowsEvaluator::Handle(Declaration& lhs, int rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  if (is_FollowT) {
    std::unordered_set < statementNumber > results = pkb_reader.followStar(stmt_entity, rhs);
    return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
  }
  std::unordered_set < statementNumber > results = pkb_reader.follows(stmt_entity, rhs);
  return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
}
Constraint FollowsEvaluator::Handle(Declaration& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  if (lhs == rhs) {
    return UnaryConstraint{lhs.synonym, {}};
  }
  StmtEntity lhs_stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  StmtEntity rhs_stmt_entity = ConvertToStmtEntity(rhs.design_entity);
  if (is_FollowT) {
    std::unordered_set < std::pair<statementNumber, statementNumber>, PairHash > raw_results
                                                                          = pkb_reader.followStar(lhs_stmt_entity,
                                                                                                  rhs_stmt_entity);
    return BinaryConstraint{{lhs.synonym, rhs.synonym}, EvaluatorUtil::ToStringPairSet(raw_results)};
  }
  std::unordered_set < std::pair<statementNumber, statementNumber>, PairHash > raw_results
                                                                        = pkb_reader.follows(lhs_stmt_entity,
                                                                                             rhs_stmt_entity);
  return BinaryConstraint{{lhs.synonym, rhs.synonym}, EvaluatorUtil::ToStringPairSet(raw_results)};
}
UnaryConstraint FollowsEvaluator::Handle(Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  if (is_FollowT) {
    std::unordered_set < statementNumber > results = pkb_reader.followStar(stmt_entity, rhs);
    return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
  }
  std::unordered_set < statementNumber > results = pkb_reader.follows(stmt_entity, rhs);
  return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
}
bool FollowsEvaluator::Handle(Wildcard& lhs, int rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  if (is_FollowT) {
    return pkb_reader.isFollowStar(lhs, rhs);
  }
  return pkb_reader.isFollow(lhs, rhs);
}
UnaryConstraint FollowsEvaluator::Handle(Wildcard& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(rhs.design_entity);
  if (is_FollowT) {
    std::unordered_set < statementNumber > results = pkb_reader.followStar(lhs, stmt_entity);
    return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
  }
  std::unordered_set < statementNumber > results = pkb_reader.follows(lhs, stmt_entity);
  return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
}
bool FollowsEvaluator::Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  if (is_FollowT) {
    return pkb_reader.isFollowStar(lhs, rhs);
  }
  return pkb_reader.isFollow(lhs, rhs);
}
