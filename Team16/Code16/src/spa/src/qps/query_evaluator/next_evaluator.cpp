#include "qps/query_evaluator/next_evaluator.h"

bool NextEvaluator::Handle(int lhs, int rhs, ReadFacade& pkb_reader, bool is_NextT) {
  if (is_NextT) {
    return pkb_reader.isNextStar(lhs, rhs);
  }
  return pkb_reader.isNext(lhs, rhs);
}
UnaryConstraint NextEvaluator::Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_NextT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(rhs.design_entity);
  if (is_NextT) {
    std::unordered_set<statementNumber> results = pkb_reader.NextStar(lhs, stmt_entity);
    return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
  }
  std::unordered_set<statementNumber> results = pkb_reader.Next(lhs, stmt_entity);
  return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
}
bool NextEvaluator::Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_NextT) {
  if (is_NextT) {
    return pkb_reader.isNextStar(lhs, rhs);
  }
  return pkb_reader.isNext(lhs, rhs);
}
UnaryConstraint NextEvaluator::Handle(Declaration& lhs, int rhs, ReadFacade& pkb_reader, bool is_NextT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  if (is_NextT) {
    std::unordered_set<statementNumber> results = pkb_reader.NextStar(stmt_entity, rhs);
    return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
  }
  std::unordered_set<statementNumber> results = pkb_reader.Next(stmt_entity, rhs);
  return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
}
Constraint NextEvaluator::Handle(Declaration& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_NextT) {
  StmtEntity lhs_stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  StmtEntity rhs_stmt_entity = ConvertToStmtEntity(rhs.design_entity);

  // Next* case
  if (is_NextT) {
    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> raw_results
        = pkb_reader.NextStar(lhs_stmt_entity,
                              rhs_stmt_entity);
    // Only for Next*, there will be cases where lhs == rhs
    if (lhs == rhs) {
      std::unordered_set<int> unary_result;
      for (const auto& p : raw_results) {
        if (p.first == p.second) {
          unary_result.insert(p.first);
        }
      }
      return UnaryConstraint{lhs.synonym, EvaluatorUtil::ToStringSet(unary_result)};
    } else {
      return BinaryConstraint{{lhs.synonym, rhs.synonym}, EvaluatorUtil::ToStringPairSet(raw_results)};
    }
  }

  // Next case
  if (lhs == rhs) {
    return UnaryConstraint{lhs.synonym, {}};
  }
  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> raw_results
      = pkb_reader.Next(lhs_stmt_entity,
                        rhs_stmt_entity);
  return BinaryConstraint{{lhs.synonym, rhs.synonym}, EvaluatorUtil::ToStringPairSet(raw_results)};
}
UnaryConstraint NextEvaluator::Handle(Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_NextT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  if (is_NextT) {
    std::unordered_set<statementNumber> results = pkb_reader.NextStar(stmt_entity, rhs);
    return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
  }
  std::unordered_set<statementNumber> results = pkb_reader.Next(stmt_entity, rhs);
  return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
}
bool NextEvaluator::Handle(Wildcard& lhs, int rhs, ReadFacade& pkb_reader, bool is_NextT) {
  if (is_NextT) {
    return pkb_reader.isNextStar(lhs, rhs);
  }
  return pkb_reader.isNext(lhs, rhs);
}
UnaryConstraint NextEvaluator::Handle(Wildcard& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_NextT) {
  StmtEntity stmt_entity = ConvertToStmtEntity(rhs.design_entity);
  if (is_NextT) {
    std::unordered_set<statementNumber> results = pkb_reader.NextStar(lhs, stmt_entity);
    return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
  }
  std::unordered_set<statementNumber> results = pkb_reader.Next(lhs, stmt_entity);
  return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
}
bool NextEvaluator::Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_NextT) {
  if (is_NextT) {
    return pkb_reader.isNextStar(lhs, rhs);
  }
  return pkb_reader.isNext(lhs, rhs);
}
