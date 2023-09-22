#pragma once

#include <variant>
#include <string>

#include "qps/declaration.h"

// TODO(phuccuongngo99): Please use the shared class within the project, or
// put all the types in a central place
enum class Wildcard {
  Value
};

using StmtRef = std::variant<Declaration, Wildcard, int>;
using EntRef = std::variant<Declaration, Wildcard, std::string>;
using RefParam = std::variant<StmtRef, EntRef>;
using ExprSpec = std::variant<std::string, Wildcard>;
