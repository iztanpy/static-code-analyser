#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

// Checks if both RefParam are of type StmtRef
bool SuchThatClause::are_stmt_ref(const RefParam& param_1, const RefParam& param_2) {
  return std::holds_alternative<StmtRef>(param_1) && std::holds_alternative<StmtRef>(param_2);
}

// Checks if both RefParam are of type EntRef
bool SuchThatClause::are_ent_ref(const RefParam& param_1, const RefParam& param_2) {
  return std::holds_alternative<EntRef>(param_1) && std::holds_alternative<EntRef>(param_2);
}

// Checks if both StmtRef are of type Declaration
bool SuchThatClause::are_stmt_decl(const StmtRef& param_1, const StmtRef& param_2) {
  return std::holds_alternative<Declaration>(param_1) && std::holds_alternative<Declaration>(param_2);
}

// Checks if both EntRef are of type Declaration
bool SuchThatClause::are_ent_decl(const EntRef& param_1, const EntRef& param_2) {
  return std::holds_alternative<Declaration>(param_1) && std::holds_alternative<Declaration>(param_2);
}

// Checks if both StmtRef are of type Wildcard
bool SuchThatClause::are_stmt_wildcard(const StmtRef& param_1, const StmtRef& param_2) {
  return std::holds_alternative<Wildcard>(param_1) && std::holds_alternative<Wildcard>(param_2);
}

// Checks if both EntRef are of type Wildcard
bool SuchThatClause::are_ent_wildcard(const EntRef& param_1, const EntRef& param_2) {
  return std::holds_alternative<Wildcard>(param_1) && std::holds_alternative<Wildcard>(param_2);
}

bool SuchThatClause::are_stmt_ref_equal(const RefParam& param_1, const RefParam& param_2) {
  // Check that both are StmtRef
  if (are_stmt_ref(param_1, param_2)) {
    // Extract the StmtRef
    StmtRef stmt_ref_1 = std::get<StmtRef>(param_1);
    StmtRef stmt_ref_2 = std::get<StmtRef>(param_2);
    if (are_stmt_decl(stmt_ref_1, stmt_ref_2)) {
      Declaration decl_1 = std::get<Declaration>(stmt_ref_1);
      Declaration decl_2 = std::get<Declaration>(stmt_ref_2);
      return decl_1 == decl_2;
    } else if (are_stmt_wildcard(stmt_ref_1, stmt_ref_2)) {
      return true;
    } else {
      return std::get<int>(stmt_ref_1) == std::get<int>(stmt_ref_2);
    }
  } else {
    return false;
  }
}

bool SuchThatClause::are_ent_ref_equal(const RefParam& param_1, const RefParam& param_2) {
  // Check that both are EntRef
  if (are_ent_ref(param_1, param_2)) {
    // Extract the EntRef
    EntRef ent_ref_1 = std::get<EntRef>(param_1);
    EntRef ent_ref_2 = std::get<EntRef>(param_2);
    if (are_ent_decl(ent_ref_1, ent_ref_2)) {
      Declaration decl_1 = std::get<Declaration>(ent_ref_1);
      Declaration decl_2 = std::get<Declaration>(ent_ref_2);
      return decl_1 == decl_2;
    } else if (are_ent_wildcard(ent_ref_1, ent_ref_2)) {
      return true;
    } else {
      return std::get<std::string>(ent_ref_1) == std::get<std::string>(ent_ref_2);
    }
  } else {
    return false;
  }
}

// ai-gen start (3.5, 0)
std::unordered_set<Synonym> SuchThatClause::ComputeSynonyms() const {
  std::unordered_set<Synonym> synonyms;

  // Helper lambda to extract Declaration from StmtRef or EntRef
  auto extractSynonym = [&](const RefParam& param) {
    if (std::holds_alternative<StmtRef>(param)) {
      const StmtRef& stmt = std::get<StmtRef>(param);
      if (std::holds_alternative<Declaration>(stmt)) {
        synonyms.insert(std::get<Declaration>(stmt).synonym);
      }
    } else if (std::holds_alternative<EntRef>(param)) {
      const EntRef& ent = std::get<EntRef>(param);
      if (std::holds_alternative<Declaration>(ent)) {
        synonyms.insert(std::get<Declaration>(ent).synonym);
      }
    }
  };

  extractSynonym(lhs);
  extractSynonym(rhs);

  return synonyms;
}
// ai-gen end

size_t SuchThatClause::Hash() const {
  uint64_t result = Clause::Hash();
  result = result * 31 + std::hash<RefParam>{}(lhs);
  result = result * 31 + std::hash<RefParam>{}(rhs);
  return static_cast<size_t>(result);
}

bool operator==(const SuchThatClause& lhs, const SuchThatClause& rhs) {
  return lhs.lhs == rhs.lhs && lhs.rhs == rhs.rhs;
}

bool SuchThatClause::equals(const Clause* other) const {
  if (!Clause::equals(other)) {
    return false;
  }
  const auto* other_clause = dynamic_cast<const SuchThatClause*>(other);
  return other_clause != nullptr && *this == *other_clause;
}
