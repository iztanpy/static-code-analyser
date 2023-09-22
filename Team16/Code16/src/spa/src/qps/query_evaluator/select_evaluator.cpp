#include "qps/query_evaluator/select_evaluator.h"

SelectEvaluator::SelectEvaluator(ReadFacade& pkb_reader, const SelectClause select_clause)
    : pkb_reader(pkb_reader), select_clause(select_clause) {}

UnaryConstraint SelectEvaluator::Evaluate() {
  auto values = [&]() -> std::unordered_set<std::string> {
    switch (select_clause.design_entity) {
      case DesignEntity::STMT:
      case DesignEntity::READ:
      case DesignEntity::PRINT:
      case DesignEntity::CALL:
      case DesignEntity::WHILE_LOOP:
      case DesignEntity::IF_STMT:
      case DesignEntity::ASSIGN: {
        std::unordered_set<int> result = pkb_reader.getStatements(ConvertToStmtEntity(select_clause.design_entity));
        return EvaluatorUtil::ToStringSet(result);
      }
      case DesignEntity::CONSTANT:return pkb_reader.getConstants();
      case DesignEntity::VARIABLE:return pkb_reader.getVariables();
      case DesignEntity::PROCEDURE:return pkb_reader.getProcedures();
    }
  }();

  return UnaryConstraint{
      select_clause.synonym,
      values
  };
}
