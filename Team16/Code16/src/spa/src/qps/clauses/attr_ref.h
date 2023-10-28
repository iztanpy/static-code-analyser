#pragma once

#include <utility>
#include <variant>
#include <unordered_map>
#include <string>
#include <unordered_set>

#include "qps/declaration.h"
#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "PKB/API/ReadFacade.h"

/*!
 * Represents a type of attrName in PQL grammar
 */
enum class AttrName {
  PROCNAME,
  VARNAME,
  VALUE,
  STMTNUM,
  NONE
};

/*!
 * Represents the underlying type of Ref in PQL grammar
 * INTEGER: int
 * IDENT: string
 */
enum class RefUnderlyingType {
  INTEGER,
  IDENT,
};

static const std::unordered_map<AttrName, std::unordered_set<DesignEntity>> kAttrNameToDesignEntity = {
    {AttrName::PROCNAME, {DesignEntity::PROCEDURE, DesignEntity::CALL}},
    {AttrName::VARNAME, {DesignEntity::VARIABLE, DesignEntity::READ, DesignEntity::PRINT}},
    {AttrName::VALUE, {DesignEntity::CONSTANT}},
    {AttrName::STMTNUM, {DesignEntity::STMT, DesignEntity::READ, DesignEntity::PRINT, DesignEntity::CALL,
                         DesignEntity::WHILE_LOOP, DesignEntity::IF_STMT, DesignEntity::ASSIGN}},
};

static const std::unordered_map<AttrName, RefUnderlyingType> kAttrNameToUnderlyingType = {
    {AttrName::PROCNAME, RefUnderlyingType::IDENT},
    {AttrName::VARNAME, RefUnderlyingType::IDENT},
    {AttrName::VALUE, RefUnderlyingType::INTEGER},
    {AttrName::STMTNUM, RefUnderlyingType::INTEGER},
};

class AttrRef {
 public:
  Declaration declaration;
  AttrName attr_name;

  AttrRef(Declaration declaration, AttrName attr_name) : declaration(std::move(declaration)), attr_name(attr_name) {
    Validate();
  }

  bool operator==(const AttrRef& other) const;

  Constraint Evaluate(ReadFacade& pkb_reader);

  size_t Hash() const;

  std::string GetSynonym() const;

 private:
  void Validate();
};

/*!
 * Represents Ref in PQL grammar
 */
using Ref = std::variant<std::string, int, AttrRef>;

// Hash Visitor for Ref
struct RefHashVisitor {
  size_t operator()(const int value) const {
    return std::hash<int>()(value);
  }
  size_t operator()(const std::string& value) const {
    return std::hash<std::string>()(value);
  }
  size_t operator()(const AttrRef& value) const {
    return value.Hash();
  }
};

// Custom std::hash for Ref
namespace std {
template<>
struct hash<Ref> {
  size_t operator()(const Ref& value) const {
    return std::visit(RefHashVisitor(), value);
  }
};
}  // namespace std
