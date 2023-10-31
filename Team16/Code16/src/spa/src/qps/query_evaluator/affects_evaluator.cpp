#include "qps/query_evaluator/affects_evaluator.h"

bool AffectsEvaluator::Handle(int lhs, int rhs, ReadFacade& pkb_reader) {
  return pkb_reader.isAffects(lhs, rhs);
}

bool AffectsEvaluator::Handle(Wildcard& lhs, int rhs, ReadFacade& pkb_reader) {
  return pkb_reader.isAffects(lhs, rhs);
}

bool AffectsEvaluator::Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader) {
  return pkb_reader.isAffects(lhs, rhs);
}

bool AffectsEvaluator::Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader) {
  return pkb_reader.isAffects(lhs, rhs);
}

UnaryConstraint AffectsEvaluator::Handle(Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader) {
  StmtEntity stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  std::unordered_set<statementNumber> results = pkb_reader.Affects(stmt_entity, rhs);
  return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
}

UnaryConstraint AffectsEvaluator::Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader) {
  StmtEntity stmt_entity = ConvertToStmtEntity(rhs.design_entity);
  std::unordered_set<statementNumber> results = pkb_reader.Affects(lhs, stmt_entity);
  return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
}

UnaryConstraint AffectsEvaluator::Handle(Declaration& lhs, int rhs, ReadFacade& pkb_reader) {
  StmtEntity stmt_entity = ConvertToStmtEntity(lhs.design_entity);
  std::unordered_set<statementNumber> results = pkb_reader.Affects(stmt_entity, rhs);
  return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
}

UnaryConstraint AffectsEvaluator::Handle(Wildcard& lhs, Declaration& rhs, ReadFacade& pkb_reader) {
  StmtEntity stmt_entity = ConvertToStmtEntity(rhs.design_entity);
  std::unordered_set<statementNumber> results = pkb_reader.Affects(lhs, stmt_entity);
  return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
}

Constraint AffectsEvaluator::Handle(Declaration& lhs, Declaration& rhs, ReadFacade& pkb_reader) {
  if (lhs.design_entity != DesignEntity::ASSIGN || rhs.design_entity != DesignEntity::ASSIGN) {
    if (lhs == rhs) {
      return UnaryConstraint{lhs.synonym, {}};
    } else {
      return BinaryConstraint{{lhs.synonym, rhs.synonym}, {}};
    }
  }

  // For Affects, there will be cases where lhs == rhs
  std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> raw_results
      = pkb_reader.Affects();

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
