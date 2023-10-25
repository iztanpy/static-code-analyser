#include "with_clause.h"

std::unordered_set<Synonym> WithClause::GetSynonyms() const {
  std::unordered_set < Synonym > synonyms;
  if (std::holds_alternative<AttrRef>(lhs)) {
    synonyms.insert(std::get<AttrRef>(lhs).declaration.synonym);
  }
  if (std::holds_alternative<AttrRef>(rhs)) {
    synonyms.insert(std::get<AttrRef>(rhs).declaration.synonym);
  }
  return synonyms;
}

Constraint WithClause::Evaluate(ReadFacade & pkb_reader) {
  return Constraint();
}

size_t WithClause::Hash() const {
  return Clause::Hash();
}

bool WithClause::equals(const Clause *other) const {
  const auto* other_clause = dynamic_cast<const WithClause*>(other);
  return other_clause != nullptr && *this == *other_clause;
}

bool operator==(const WithClause& first, const WithClause& second) {
  bool is_rhs_equal;
  bool is_lhs_equal;
  // first check the equality of lhs Ref
  if (std::holds_alternative<std::string>(first.lhs) && std::holds_alternative<std::string>(second.lhs)) {
    is_lhs_equal = (std::get<std::string>(first.lhs) == std::get<std::string>(second.lhs));
  } else {
    AttrRef first_lhs = std::get<AttrRef>(first.lhs);
    AttrRef second_lhs = std::get<AttrRef>(second.lhs);
    is_lhs_equal = (first_lhs.attr_name == second_lhs.attr_name
        && first_lhs.declaration.operator==(second_lhs.declaration));
  }
  // Check the equality of rhs Ref
  if (std::holds_alternative<std::string>(first.rhs) && std::holds_alternative<std::string>(second.rhs)) {
    is_rhs_equal = (std::get<std::string>(first.rhs) == std::get<std::string>(second.rhs));
  } else {
    AttrRef first_rhs = std::get<AttrRef>(first.rhs);
    AttrRef second_rhs = std::get<AttrRef>(second.rhs);
    is_rhs_equal = (first_rhs.attr_name == second_rhs.attr_name
        && first_rhs.declaration.operator==(second_rhs.declaration));
  }

  return is_lhs_equal && is_rhs_equal;
}
void WithClause::Validate() {
  // TODO: Implement validate
}
