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

Constraint AttrRef::Evaluate(ReadFacade& pkb_reader) {
  if (IsComplexCase()) {
    auto synonyms = GetSynonyms();
    auto it = synonyms.begin();
    std::unordered_set<std::pair<int, std::string>, PairHash>
        raw_results = pkb_reader.getStatementsAndVariable(ConvertToStmtEntity(declaration.design_entity));
    return BinaryConstraint{std::make_pair(*it, *(++it)), EvaluatorUtil::ToStringPairSet(raw_results)};
  } else {
    auto values = [&]() -> std::unordered_set<std::string> {
      switch (declaration.design_entity) {
        case DesignEntity::STMT:
        case DesignEntity::READ:
        case DesignEntity::PRINT:
        case DesignEntity::CALL:
        case DesignEntity::WHILE_LOOP:
        case DesignEntity::ASSIGN:
        case DesignEntity::IF_STMT: {
          std::unordered_set<int> result = pkb_reader.getStatements(ConvertToStmtEntity(declaration.design_entity));
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
}

size_t AttrRef::Hash() const {
  uint64_t result = std::hash<Declaration>()(declaration);
  result = result * 31 + std::hash<AttrName>()(attr_name);
  return static_cast<size_t>(result);
}

std::unordered_set<Synonym> AttrRef::GetSynonyms() const {
  if (IsComplexCase()) {
    return {declaration.synonym, declaration.synonym + kAttrSynonym};
  } else {
    return {declaration.synonym};
  }
}

bool AttrRef::IsComplexCase() const {
  return
      (declaration.design_entity == DesignEntity::CALL && attr_name == AttrName::PROCNAME)
          || (declaration.design_entity == DesignEntity::READ && attr_name == AttrName::VARNAME)
          || (declaration.design_entity == DesignEntity::PRINT && attr_name == AttrName::VARNAME);
}
