#include "qps/clauses/suchthat_clauses/uses_s.h"

struct UsesVisitor {
  static UnaryConstraint handle(int stmt_num, Declaration& declaration, ReadFacade& pkb_reader) {
    if (declaration.design_entity == DesignEntity::VARIABLE) {
      std::unordered_set<std::string> result = pkb_reader.getVariablesUsedBy(stmt_num);
      return UnaryConstraint{declaration.synonym, result};
    } else if (declaration.design_entity == DesignEntity::CONSTANT) {
      return UnaryConstraint{declaration.synonym, pkb_reader.getConstantsUsedBy(stmt_num)};
    } else {
      throw QpsSemanticError("Not implemented");
    }
  }

  static Constraint handle(int stmt_num, Wildcard& wildcard, ReadFacade& read_facade) {
    throw QpsSemanticError("Not implemented");
  }
  static Constraint handle(int stmt_num, std::string& entity_name, ReadFacade& read_facade) {
    throw QpsSemanticError("Not implemented");
  }

  static Constraint handle(Declaration& declaration1, Declaration& declaration2, ReadFacade& pkb_reader) {
    throw QpsSemanticError("Not implemented");
  }

  static Constraint handle(Declaration& declaration, Wildcard& wildcard, ReadFacade& pkb_reader) {
    throw QpsSemanticError("Not implemented");
  }

  static Constraint handle(Declaration& declaration, std::string& entity_name, ReadFacade& pkb_reader) {
    throw QpsSemanticError("Not implemented");
  }

  static Constraint handle(Wildcard& wildcard, Declaration& declaration, ReadFacade& pkb_reader) {
    throw QpsSemanticError("Not implemented");
  }

  static Constraint handle(Wildcard& wildcard1, Wildcard& wildcard2, ReadFacade& pkb_reader) {
    throw QpsSemanticError("Not implemented");
  }

  static Constraint handle(Wildcard& wildcard, std::string& entity_name, ReadFacade& pkb_reader) {
    throw QpsSemanticError("Not implemented");
  }
};

UsesS::UsesS(StmtRef lhs, EntRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
}

Constraint UsesS::Evaluate(ReadFacade& pkb_reader) {
//  throw QpsSemanticError("Not implemented");
  return std::visit([&pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{UsesVisitor::handle(lhs_arg, rhs_arg, pkb_reader)};
  }, lhs, rhs);
}
