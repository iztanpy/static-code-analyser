#include "qps/clauses/suchthat_clauses/uses.h"

// TODO(phuccuongngo99): Move this constant to some shared file
const bool kIsBoolConstraint = true;

// TODO(phuccuongngo99): Consider writing header file for this
struct UsesVisitor {
  static UnaryConstraint handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader) {
    std::unordered_set<std::string> result = pkb_reader.getVariablesUsedBy(lhs);
    return UnaryConstraint{rhs.synonym, result};
  }

  static bool handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader) {
    throw QpsSemanticError("[Uses] Not implemented");
  }

  static bool handle(int lhs, std::string& rhs, ReadFacade& pkb_reader) {
    throw QpsSemanticError("[Uses] Not implemented");
  }

  static BinaryConstraint handle(Declaration& lhs,
                                 Declaration& rhs,
                                 ReadFacade& pkb_reader) {
    throw QpsSemanticError("[Uses] Not implemented");
  }

  static UnaryConstraint handle(Declaration& lhs,
                                Wildcard& rhs,
                                ReadFacade& pkb_reader) {
    throw QpsSemanticError("[Uses] Not implemented");
  }

  static UnaryConstraint handle(Declaration& lhs,
                                std::string& rhs,
                                ReadFacade& pkb_reader) {
    throw QpsSemanticError("[Uses] Not implemented");
  }

  static UnaryConstraint handle(Wildcard& lhs,
                                Declaration& rhs,
                                ReadFacade& pkb_reader) {
    assert(false && "[Uses] This code should never be reached! Make sure to call Validate() within constructor");
  }

  static bool handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader) {
    assert(false && "[Uses] This code should never be reached! Make sure to call Validate() within constructor");
  }

  static bool handle(Wildcard& lhs, std::string& rhs, ReadFacade& pkb_reader) {
    assert(false && "[Uses] This code should never be reached! Make sure to call Validate() within constructor");
  }

  static UnaryConstraint handle(std::string& lhs_procname,
                                Declaration& rhs,
                                ReadFacade& pkb_reader) {
    throw QpsSemanticError("[Uses] Not required by Milestone1");
  }

  static bool handle(std::string& lhs_proc_name, Wildcard& rhs, ReadFacade& pkb_reader) {
    throw QpsSemanticError("[Uses] Not required by Milestone1");
  }

  static bool handle(std::string& lhs_proc_name, std::string& rhs, ReadFacade& pkb_reader) {
    throw QpsSemanticError("[Uses] Not required by Milestone1");
  }
};

static const std::unordered_set<DesignEntity> kValidRhsDeclaration = {DesignEntity::VARIABLE};
static const std::unordered_set<DesignEntity> kValidLhsDeclaration =
    {DesignEntity::STMT, DesignEntity::ASSIGN, DesignEntity::PRINT, DesignEntity::IF_STMT,
     DesignEntity::WHILE_LOOP, DesignEntity::CALL, DesignEntity::PROCEDURE};

Constraint UsesS::Evaluate(ReadFacade& pkb_reader) {
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{UsesVisitor::handle(lhs_arg, rhs_arg, pkb_reader)};
  }, this->lhs, this->rhs);
}

// We want the constructor to call this, this should be private
// we want this to throw exception, so constructor can catch it
// and throw it back. So whoever calls the constructor will get the exception
void UsesS::Validate() {
  SuchThatValidator::ValidateUses(lhs, rhs);
}

Constraint UsesP::Evaluate(ReadFacade& pkb_reader) {
//  throw QpsSemanticError("Not implemented");
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{UsesVisitor::handle(lhs_arg, rhs_arg, pkb_reader)};
  }, this->lhs, this->rhs);
}

void UsesP::Validate() {
  SuchThatValidator::ValidateUses(lhs, rhs);
}
