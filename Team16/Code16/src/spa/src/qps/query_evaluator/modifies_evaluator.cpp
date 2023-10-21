#include "qps/query_evaluator/modifies_evaluator.h"

UnaryConstraint ModifiesEvaluator::Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader) {
  return {rhs.synonym, pkb_reader.relates(lhs)};
}

bool ModifiesEvaluator::Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader) {
  return pkb_reader.isRelation(lhs, rhs);
}

bool ModifiesEvaluator::Handle(int lhs, std::string& rhs, ReadFacade& pkb_reader) {
  return pkb_reader.isRelation(lhs, rhs);
}

Constraint ModifiesEvaluator::Handle(Declaration& lhs,
                                     Declaration& rhs,
                                     ReadFacade& pkb_reader) {
  if (lhs == rhs) {
    return UnaryConstraint{lhs.synonym, {}};
  }

  if (lhs.design_entity == DesignEntity::PROCEDURE) {
    return BinaryConstraint{{lhs.synonym, rhs.synonym},
                            pkb_reader.relatesProcedure()};
  }

  std::unordered_set < std::pair<statementNumber, variable>, PairHash >
      raw_results = pkb_reader.relates(ConvertToStmtEntity(lhs.design_entity));
  return BinaryConstraint{{lhs.synonym, rhs.synonym}, EvaluatorUtil::ToStringPairSet(raw_results)};
}

UnaryConstraint ModifiesEvaluator::Handle(Declaration& lhs,
                                          Wildcard& rhs,
                                          ReadFacade& pkb_reader) {
  if (lhs.design_entity == DesignEntity::PROCEDURE) {
    return {lhs.synonym, pkb_reader.relatesProcedure(rhs)};
  }

  std::unordered_set < statementNumber > results = pkb_reader.relates(ConvertToStmtEntity(lhs.design_entity), rhs);
  return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
}

UnaryConstraint ModifiesEvaluator::Handle(Declaration& lhs,
                                          std::string& rhs,
                                          ReadFacade& pkb_reader) {
  if (lhs.design_entity == DesignEntity::PROCEDURE) {
    return {lhs.synonym, pkb_reader.relatesProcedure(rhs)};
  }

  std::unordered_set < statementNumber > results = pkb_reader.relates(ConvertToStmtEntity(lhs.design_entity), rhs);
  return {lhs.synonym, EvaluatorUtil::ToStringSet(results)};
}

UnaryConstraint ModifiesEvaluator::Handle(Wildcard& lhs,
                                          Declaration& rhs,
                                          ReadFacade& pkb_reader) {
  assert(false && "[Modifies] This code should never be reached! Make sure to call Validate() within constructor");
  return UnaryConstraint{rhs.synonym, {}};
}

bool ModifiesEvaluator::Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader) {
  assert(false && "[Modifies] This code should never be reached! Make sure to call Validate() within constructor");
  return false;
}

bool ModifiesEvaluator::Handle(Wildcard& lhs, std::string& rhs, ReadFacade& pkb_reader) {
  assert(false && "[Modifies] This code should never be reached! Make sure to call Validate() within constructor");
  return false;
}

UnaryConstraint ModifiesEvaluator::Handle(std::string& lhs_procname,
                                          Declaration& rhs,
                                          ReadFacade& pkb_reader) {
  return {rhs.synonym, pkb_reader.relates(lhs_procname)};
}

bool ModifiesEvaluator::Handle(std::string& lhs_proc_name, Wildcard& rhs, ReadFacade& pkb_reader) {
  return pkb_reader.isRelation(lhs_proc_name, rhs);
}

bool ModifiesEvaluator::Handle(std::string& lhs_proc_name, std::string& rhs, ReadFacade& pkb_reader) {
  return pkb_reader.isRelation(lhs_proc_name, rhs);
}
