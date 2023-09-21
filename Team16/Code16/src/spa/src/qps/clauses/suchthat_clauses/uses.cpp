#include "qps/clauses/suchthat_clauses/uses.h"

// TODO(phuccuongngo99): Move this constant to some shared file
const bool kIsBoolConstraint = true;
using MaybeReadFacade = std::optional<std::reference_wrapper<ReadFacade>>;

// TODO(phuccuongngo99): Consider writing header file for this
struct UsesVisitor {
  inline static const std::unordered_set<DesignEntity> kValidRhsDeclaration = {DesignEntity::VARIABLE};
  inline static const std::unordered_set<DesignEntity> kValidLhsDeclaration =
      {DesignEntity::STMT, DesignEntity::ASSIGN, DesignEntity::PRINT, DesignEntity::IF_STMT,
       DesignEntity::WHILE_LOOP, DesignEntity::CALL, DesignEntity::PROCEDURE};

  static UnaryConstraint handle(int lhs, Declaration& rhs, MaybeReadFacade maybe_pkb_reader) {
    if (kValidRhsDeclaration.find(rhs.design_entity) == kValidRhsDeclaration.end()) {
      throw QpsSemanticError("[Uses] Invalid RHS synonym");
    }

    if (!maybe_pkb_reader) {
      return UnaryConstraint{rhs.synonym, std::unordered_set<std::string>{}};
    }

    ReadFacade& pkb_reader = maybe_pkb_reader.value().get();
    std::unordered_set<std::string> result = pkb_reader.getVariablesUsedBy(lhs);
    return UnaryConstraint{rhs.synonym, result};
  }

  static bool handle(int lhs, Wildcard& rhs, MaybeReadFacade maybe_pkb_reader) {
    throw QpsSemanticError("[Uses] Not implemented");
  }

  static bool handle(int lhs, std::string& rhs, MaybeReadFacade maybe_pkb_reader) {
    throw QpsSemanticError("[Uses] Not implemented");
  }

  static BinaryConstraint handle(Declaration& lhs,
                                 Declaration& rhs,
                                 MaybeReadFacade maybe_pkb_reader) {
    if (kValidLhsDeclaration.find(lhs.design_entity) == kValidLhsDeclaration.end()) {
      throw QpsSemanticError("[Uses] Invalid LHS synonym");
    }

    if (kValidRhsDeclaration.find(rhs.design_entity) == kValidRhsDeclaration.end()) {
      throw QpsSemanticError("[Uses] Invalid RHS synonym");
    }

    if (!maybe_pkb_reader) {
      return BinaryConstraint{{rhs.synonym, lhs.synonym}, {}};
    }
    throw QpsSemanticError("[Uses] Not implemented");
  }

  static UnaryConstraint handle(Declaration& lhs,
                                Wildcard& rhs,
                                MaybeReadFacade maybe_pkb_reader) {
    if (kValidLhsDeclaration.find(lhs.design_entity) == kValidLhsDeclaration.end()) {
      throw QpsSemanticError("[Uses] Invalid LHS synonym");
    }

    if (!maybe_pkb_reader) {
      return UnaryConstraint{lhs.synonym, {}};
    }
    throw QpsSemanticError("[Uses] Not implemented");
  }

  static UnaryConstraint handle(Declaration& lhs,
                                std::string& rhs,
                                MaybeReadFacade maybe_pkb_reader) {
    if (kValidLhsDeclaration.find(lhs.design_entity) == kValidLhsDeclaration.end()) {
      throw QpsSemanticError("[Uses] Invalid LHS synonym");
    }

    if (!maybe_pkb_reader) {
      return UnaryConstraint{lhs.synonym, {}};
    }
    throw QpsSemanticError("[Uses] Not implemented");
  }

  static UnaryConstraint handle(Wildcard& lhs,
                                Declaration& rhs,
                                MaybeReadFacade maybe_pkb_reader) {
    throw QpsSemanticError("[Uses] LHS cannot be wildcard");
  }

  static bool handle(Wildcard& lhs, Wildcard& rhs, MaybeReadFacade maybe_pkb_reader) {
    throw QpsSemanticError("[Uses] LHS cannot be wildcard");
  }

  static bool handle(Wildcard& lhs, std::string& rhs, MaybeReadFacade maybe_pkb_reader) {
    throw QpsSemanticError("[Uses] LHS cannot be wildcard");
  }

  static UnaryConstraint handle(std::string& lhs_procname,
                                Declaration& rhs,
                                MaybeReadFacade maybe_pkb_reader) {
    throw QpsSemanticError("[Uses] Not required by Milestone1");
    if (kValidRhsDeclaration.find(rhs.design_entity) == kValidRhsDeclaration.end()) {
      throw QpsSemanticError("[Uses] Invalid RHS synonym");
    }
  }

  static bool handle(std::string& lhs_proc_name, Wildcard& rhs, MaybeReadFacade maybe_pkb_reader) {
    throw QpsSemanticError("[Uses] Not required by Milestone1");
  }

  static bool handle(std::string& lhs_proc_name, std::string& rhs, MaybeReadFacade maybe_pkb_reader) {
    throw QpsSemanticError("[Uses] Not required by Milestone1");
  }
};

UsesS::UsesS(StmtRef lhs, EntRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);

  try {
    Validate();
  } catch (const QpsSemanticError& e) {
    throw;
  }
}

Constraint UsesS::Evaluate(ReadFacade& pkb_reader) {
//  throw QpsSemanticError("Not implemented");
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{UsesVisitor::handle(lhs_arg, rhs_arg, pkb_reader)};
  }, this->lhs, this->rhs);
}

// We want the constructor to call this, this should be private
// we want this to throw exception, so constructor can catch it
// and throw it back. So whoever calls the constructor will get the exception
void UsesS::Validate() {
  try {
    std::visit([this](auto&& lhs_arg, auto&& rhs_arg) {
      UsesVisitor::handle(lhs_arg, rhs_arg, std::nullopt);
    }, this->lhs, this->rhs);
  } catch (const QpsSemanticError& e) {
    // Optionally, you can handle or log the error here
    throw;  // Rethrow the caught exception
  }
}

UsesP::UsesP(EntRef lhs, EntRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);

  try {
    Validate();
  } catch (const QpsSemanticError& e) {
    throw;
  }
}

Constraint UsesP::Evaluate(ReadFacade& pkb_reader) {
//  throw QpsSemanticError("Not implemented");
  return std::visit([this, &pkb_reader](auto&& lhs_arg, auto&& rhs_arg) {
    return Constraint{UsesVisitor::handle(lhs_arg, rhs_arg, pkb_reader)};
  }, this->lhs, this->rhs);
}

void UsesP::Validate() {
  try {
    std::visit([this](auto&& lhs_arg, auto&& rhs_arg) {
      UsesVisitor::handle(lhs_arg, rhs_arg, std::nullopt);
    }, this->lhs, this->rhs);
  } catch (const QpsSemanticError& e) {
    // Optionally, you can handle or log the error here
    throw;  // Rethrow the caught exception
  }
}
