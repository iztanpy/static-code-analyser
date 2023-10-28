#include "qps/clauses/attr_ref.h"

bool AttrRef::operator==(const AttrRef& other) const {
  return declaration == other.declaration && attr_name == other.attr_name;
}

void AttrRef::Validate() {
  if (attr_name == AttrName::NONE) {
    return;
  }
  if (kAttrNameToDesignEntity.at(attr_name).find(declaration.design_entity) ==
      kAttrNameToDesignEntity.at(attr_name).end()) {
    throw QpsSemanticError("Invalid AttrName for synonym");
  }
}

// TODO(phuccuongngo99): Implement this
Constraint AttrRef::Evaluate(ReadFacade& pkb_reader) {
  return Constraint{};
}

size_t AttrRef::Hash() const {
  uint64_t result = std::hash<Declaration>()(declaration);
  result = result * 31 + std::hash<AttrName>()(attr_name);
  return static_cast<size_t>(result);
}
Synonym AttrRef::GetSynonym() const {
  return declaration.synonym;
}
