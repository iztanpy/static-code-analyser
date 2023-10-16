#pragma once

#include <variant>
#include <string>

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

struct PartialExpr {
  std::string value;
};

struct ExactExpr {
  std::string value;
};

/*!
 * Represents a type that a pattern expressions can take in PQL grammar
 */
using ExprSpec = std::variant<PartialExpr, ExactExpr, Wildcard>;
using synonym = std::string;

