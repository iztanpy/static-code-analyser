#pragma once

#include <variant>
#include <string>
#include <functional>
#include <unordered_set>

#include "qps/declaration.h"


/*!
 * Represents StmtRef in PQL grammar
 */
using StmtRef = std::variant<Declaration, Wildcard, int>;

/*!
 * Represents EntRef in PQL grammar
 */
using EntRef = std::variant<Declaration, Wildcard, std::string>;

/*!
 * Represents a type that clauses can take in
 */
using RefParam = std::variant<StmtRef, EntRef>;

enum AttrName {
  PROCNAME,
  VARNAME,
  VALUE,
  STMTNUM,
  NONE
};

struct AttrRef {
  Declaration declaration;
  AttrName attr_name;

  bool operator==(const AttrRef& other) const {
    return declaration == other.declaration && attr_name == other.attr_name;
  }
};

/*!
 * Represents Ref in PQL grammar
 */
using Ref = std::variant<std::string, int, AttrRef>;

struct PartialExpr {
  std::string value;

  bool operator==(const PartialExpr& other) const {
    return value == other.value;
  }
};

struct ExactExpr {
  std::string value;

  bool operator==(const ExactExpr& other) const {
    return value == other.value;
  }
};

/*!
 * Represents a type that a pattern expressions can take in PQL grammar
 */
using ExprSpec = std::variant<PartialExpr, ExactExpr, Wildcard>;
using Synonym = std::string;

/*!
 * Hash function for EntRef and StmtRef
 */
struct HashVisitor {
  size_t operator()(const Declaration& decl) const {
    return std::hash<Declaration>()(decl);
  }

  size_t operator()(const Wildcard& wc) const {
    return std::hash<int>()(static_cast<int>(wc));
  }

  size_t operator()(const int& value) const {
    return std::hash<int>()(value);
  }

  size_t operator()(const std::string& str) const {
    return std::hash<std::string>()(str);
  }
};

namespace std {
// Custom std::hash for EntRef
template<>
struct hash<EntRef> {
  size_t operator()(const EntRef& ref) const {
    return std::visit(HashVisitor(), ref);
  }
};

template<>
struct hash<StmtRef> {
  size_t operator()(const StmtRef& ref) const {
    return std::visit(HashVisitor(), ref);
  }
};
}  // namespace std

/*!
 * Hash function for RefParam
 */
// Hash Visitor for RefParam
struct RefParamHashVisitor {
  size_t operator()(const StmtRef& str) const {
    return std::hash<StmtRef>()(str);
  }
  size_t operator()(const EntRef& value) const {
    return std::hash<EntRef>()(value);
  }
};

namespace std {
// Custom std::hash for RefParam
template<>
struct hash<RefParam> {
  size_t operator()(const RefParam& ref) const {
    return std::visit(RefParamHashVisitor(), ref);
  }
};
}  // namespace std

/*!
 * Hash function for ExprSpec
 */
// Hash Visitor for ExprSpec
struct ExprSpecHashVisitor {
  size_t operator()(const PartialExpr& expr) const {
    return std::hash<std::string>()(expr.value);
  }
  size_t operator()(const ExactExpr& expr) const {
    return std::hash<std::string>()(expr.value);
  }

  size_t operator()(const Wildcard& wc) const {
    return std::hash<int>()(static_cast<int>(wc));
  }
};

namespace std {
// Custom std::hash for ExprSpec
template<>
struct hash<ExprSpec> {
  size_t operator()(const ExprSpec& spec) const {
    return std::visit(ExprSpecHashVisitor(), spec);
  }
};
}  // namespace std

/*!
 * Hash function for AttrRef and Ref
 */
namespace std {
template<>
struct hash<AttrRef> {
  size_t operator()(const AttrRef& attr_ref) const {
    size_t h1 = std::hash<Declaration>()(attr_ref.declaration);
    size_t h2 = std::hash<int>()(static_cast<int>(attr_ref.attr_name));
    return h1 ^ (h2 << 1);
  }
};
}  // namespace std

// Hash Visitor for Ref
struct RefHashVisitor {
  size_t operator()(const int value) const {
    return std::hash<int>()(value);
  }
  size_t operator()(const std::string& value) const {
    return std::hash<std::string>()(value);
  }
  size_t operator()(const AttrRef& value) const {
    return std::hash<AttrRef>()(value);
  }
};

namespace std {
// Custom std::hash for ExprSpec
template<>
struct hash<Ref> {
  size_t operator()(const Ref& value) const {
    return std::visit(RefHashVisitor(), value);
  }
};
}  // namespace std
