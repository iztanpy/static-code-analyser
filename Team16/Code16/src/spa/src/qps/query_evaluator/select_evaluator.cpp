#include "qps/query_evaluator/select_evaluator.h"

UnaryConstraint SelectEvaluator::Evaluate(Declaration& declaration, ReadFacade& pkb_reader) {
  auto values = [&]() -> std::unordered_set<std::string> {
    switch (declaration.design_entity) {
      case DesignEntity::STMT:
      case DesignEntity::READ:
      case DesignEntity::PRINT:
      case DesignEntity::CALL:
      case DesignEntity::WHILE_LOOP:
      case DesignEntity::IF_STMT: {
        std::unordered_set<int> result = pkb_reader.getStatements(ConvertToStmtEntity(declaration.design_entity));
        return EvaluatorUtil::ToStringSet(result);
      }
      case DesignEntity::ASSIGN: {
        std::unordered_set<int> result = pkb_reader.getAllAssigns();
        return EvaluatorUtil::ToStringSet(result);
      }
      case DesignEntity::CONSTANT:return pkb_reader.getConstants();
      case DesignEntity::VARIABLE:return pkb_reader.getVariables();
      case DesignEntity::PROCEDURE:return pkb_reader.getProcedures();
    }
  }();

  return UnaryConstraint{
      declaration.synonym,
      values
  };
}
