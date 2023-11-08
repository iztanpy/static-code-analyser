#include "qps/query_evaluator/affects_evaluator.h"

namespace {
bool IsDeclAffectsStmt(Declaration& declaration) {
  return declaration.design_entity == DesignEntity::ASSIGN || declaration.design_entity == DesignEntity::STMT;
}
}  // anonymous namespace

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
  if (!IsDeclAffectsStmt(lhs)) {
    return {lhs.synonym, {}};
  }
  std::unordered_set<statementNumber> results = pkb_reader.Affects(StmtEntity::kAssign, rhs);
  return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
}

UnaryConstraint AffectsEvaluator::Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader) {
  if (!IsDeclAffectsStmt(rhs)) {
    return {rhs.synonym, {}};
  }
  std::unordered_set<statementNumber> results = pkb_reader.Affects(lhs, StmtEntity::kAssign);
  return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
}

UnaryConstraint AffectsEvaluator::Handle(Declaration& lhs, int rhs, ReadFacade& pkb_reader) {
  if (!IsDeclAffectsStmt(lhs)) {
    return {lhs.synonym, {}};
  }
  std::unordered_set<statementNumber> results = pkb_reader.Affects(StmtEntity::kAssign, rhs);
  return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
}

UnaryConstraint AffectsEvaluator::Handle(Wildcard& lhs, Declaration& rhs, ReadFacade& pkb_reader) {
  if (!IsDeclAffectsStmt(rhs)) {
    return {rhs.synonym, {}};
  }
  std::unordered_set<statementNumber> results = pkb_reader.Affects(lhs, StmtEntity::kAssign);
  return {rhs.synonym, EvaluatorUtil::ToStringSet(results)};
}

Constraint AffectsEvaluator::Handle(Declaration& lhs, Declaration& rhs, ReadFacade& pkb_reader) {
  if (!IsDeclAffectsStmt(lhs) || !IsDeclAffectsStmt(rhs)) {
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
