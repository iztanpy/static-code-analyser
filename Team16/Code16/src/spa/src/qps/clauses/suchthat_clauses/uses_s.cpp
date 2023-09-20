#include "qps/clauses/suchthat_clauses/uses_s.h"

UsesS::UsesS(StmtRef lhs, EntRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint UsesS::handle(int stmt_num, Declaration& declaration, ReadFacade& pkb_reader) {
  if (declaration.design_entity == DesignEntity::VARIABLE) {
    std::unordered_set<std::string> result = pkb_reader.getVariablesUsedBy(stmt_num);
    return UnaryConstraint{declaration.synonym, result};
  } else if (declaration.design_entity == DesignEntity::CONSTANT) {
    return UnaryConstraint{declaration.synonym, pkb_reader.getConstantsUsedBy(stmt_num)};
  } else {
    throw QpsSemanticError("Not implemented");
  }
}

Constraint UsesS::handle(int stmt_num, Wildcard& wildcard, ReadFacade& read_facade) {
  throw QpsSemanticError("Not implemented");
}
Constraint UsesS::handle(int stmt_num, std::string& entity_name, ReadFacade& read_facade) {
  throw QpsSemanticError("Not implemented");
}

Constraint UsesS::Evaluate(ReadFacade& pkb_reader) {
//  throw QpsSemanticError("Not implemented");
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return this->handle(lhs_arg, rhs_arg, pkb_reader);
  }, lhs, rhs);
}
