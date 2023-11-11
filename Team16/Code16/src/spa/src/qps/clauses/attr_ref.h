#pragma once

#include <utility>
#include <variant>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>

#include "qps/declaration.h"
#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "qps/query_evaluator/evaluator_util.h"
#include "PKB/API/read_facade.h"

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

/*!
 * Represents the design entity that an AttrRef can refer to
 */
static const std::unordered_map<AttrName, std::unordered_set<DesignEntity>> kAttrNameToDesignEntity = {
    {AttrName::PROCNAME, {DesignEntity::PROCEDURE, DesignEntity::CALL}},
    {AttrName::VARNAME, {DesignEntity::VARIABLE, DesignEntity::READ, DesignEntity::PRINT}},
    {AttrName::VALUE, {DesignEntity::CONSTANT}},
    {AttrName::STMTNUM, {DesignEntity::STMT, DesignEntity::READ, DesignEntity::PRINT, DesignEntity::CALL,
                         DesignEntity::WHILE_LOOP, DesignEntity::IF_STMT, DesignEntity::ASSIGN}},
};

/*!
 * Represents the underlying type of Ref in PQL grammar
 */
static const std::unordered_map<AttrName, RefUnderlyingType> kAttrNameToUnderlyingType = {
    {AttrName::PROCNAME, RefUnderlyingType::IDENT},
    {AttrName::VARNAME, RefUnderlyingType::IDENT},
    {AttrName::VALUE, RefUnderlyingType::INTEGER},
    {AttrName::STMTNUM, RefUnderlyingType::INTEGER},
};

/*!
 * Represents AttrRef in PQL grammar
 */
class AttrRef {
 public:
  Declaration declaration;
  AttrName attr_name;

  AttrRef(Declaration declaration, AttrName attr_name) : declaration(std::move(declaration)), attr_name(attr_name) {
    Validate();
  }

  /*!
   * Checks if this AttrRef is a complex case.
   * @return true if it's call.ProcName, read.VarName, print.VarName
   */
  bool IsComplexCase() const;

  bool operator==(const AttrRef& other) const;

  /*!
   * Evaluates this AttrRef
   * @param pkb_reader is the PKB ReadFacade
   * @return a Constraint that contains all possible values of this AttrRef
   * BinaryConstraint if it's a complex case like call.ProcName, read.VarName, print.VarName
   * UnaryConstraint if it's a normal case
   */
  Constraint Evaluate(ReadFacade& pkb_reader);

  size_t Hash() const;

  /*!
   * Gets the synonyms used in this AttrRef
   * @return a vector of synonyms used in this AttrRef
   * Only 2 elements if it's Call.procName, Read.varName, Print.varName
   *    in which case the second synonym will be the synonym with .ATTR appended
   * Only 1 element if it's a normal case
   */
  std::vector<Declaration> GetSynonyms() const;

 private:
  constexpr static const char kAttrSynonym[] = ".ATTR";

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
