#include "qps/clauses/suchthat_clauses/uses.h"

// TODO(phuccuongngo99): Move this constant to some shared file
const bool kIsBoolConstraint = true;
using MaybeReadFacade = std::optional<std::reference_wrapper<ReadFacade>>;

struct UsesVisitor {
  static UnaryConstraint handle(int stmt_num, Declaration& declaration, MaybeReadFacade maybe_pkb_reader) {
    if (!maybe_pkb_reader) {
      return UnaryConstraint{declaration.synonym, std::unordered_set<std::string>{}};
    }

    ReadFacade& pkb_reader = maybe_pkb_reader.value().get();

    if (declaration.design_entity == DesignEntity::VARIABLE) {
      std::unordered_set<std::string> result = pkb_reader.getVariablesUsedBy(stmt_num);
      return UnaryConstraint{declaration.synonym, result};
    } else if (declaration.design_entity == DesignEntity::CONSTANT) {
      return UnaryConstraint{declaration.synonym, pkb_reader.getConstantsUsedBy(stmt_num)};
    } else {
      throw QpsSemanticError("Not implemented");
    }
  }

  static bool handle(int stmt_num, Wildcard& wildcard, MaybeReadFacade maybe_pkb_reader) {
    if (!maybe_pkb_reader) {
      return kIsBoolConstraint;
    }
    throw QpsSemanticError("Not implemented");
  }

  static bool handle(int stmt_num, std::string& entity_name, MaybeReadFacade maybe_pkb_reader) {
    if (!maybe_pkb_reader) {
      return kIsBoolConstraint;
    }
    throw QpsSemanticError("Not implemented");
  }

  static BinaryConstraint handle(Declaration& declaration1,
                                 Declaration& declaration2,
                                 MaybeReadFacade maybe_pkb_reader) {
    if (!maybe_pkb_reader) {
      return BinaryConstraint{{declaration1.synonym, declaration2.synonym}, {}};
    }
    throw QpsSemanticError("Not implemented");
  }

  static UnaryConstraint handle(Declaration& declaration,
                                Wildcard& wildcard,
                                MaybeReadFacade maybe_pkb_reader) {
    if (!maybe_pkb_reader) {
      return UnaryConstraint{declaration.synonym, {}};
    }
    throw QpsSemanticError("Not implemented");
  }

  static UnaryConstraint handle(Declaration& declaration,
                                std::string& entity_name,
                                MaybeReadFacade maybe_pkb_reader) {
    if (!maybe_pkb_reader) {
      return UnaryConstraint{declaration.synonym, {}};
    }
    throw QpsSemanticError("Not implemented");
  }

  static UnaryConstraint handle(Wildcard& wildcard,
                                Declaration& declaration,
                                MaybeReadFacade maybe_pkb_reader) {
    if (!maybe_pkb_reader) {
      return UnaryConstraint{declaration.synonym, {}};
    }
    throw QpsSemanticError("Not implemented");
  }

  static bool handle(Wildcard& wildcard1, Wildcard& wildcard2, MaybeReadFacade maybe_pkb_reader) {
    if (!maybe_pkb_reader) {
      return kIsBoolConstraint;
    }
    throw QpsSemanticError("Not implemented");
  }

  static bool handle(Wildcard& wildcard, std::string& entity_name, MaybeReadFacade maybe_pkb_reader) {
    if (!maybe_pkb_reader) {
      return kIsBoolConstraint;
    }
    throw QpsSemanticError("Not implemented");
  }

  static BinaryConstraint handle(std::string& proc_name,
                                 Declaration& declaration,
                                 MaybeReadFacade maybe_pkb_reader) {
    if (!maybe_pkb_reader) {
      return BinaryConstraint{{proc_name, declaration.synonym}, {}};
    }
    throw QpsSemanticError("Not required by Milestone1");
  }

  static bool handle(std::string& proc_name, Wildcard& wildcard, MaybeReadFacade maybe_pkb_reader) {
    if (!maybe_pkb_reader) {
      return kIsBoolConstraint;
    }
    throw QpsSemanticError("Not required by Milestone1");
  }

  static bool handle(std::string& proc_name, std::string& entity_name, MaybeReadFacade maybe_pkb_reader) {
    if (!maybe_pkb_reader) {
      return kIsBoolConstraint;
    }
    throw QpsSemanticError("Not required by Milestone1");
  }
};

UsesS::UsesS(StmtRef lhs, EntRef rhs) {
  this->rhs = std::move(rhs);
  this->lhs = std::move(lhs);
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
