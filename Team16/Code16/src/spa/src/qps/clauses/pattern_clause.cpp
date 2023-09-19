#pragma once

#include "pattern_clause.h"

#include <utility>
WildCardPattern::WildCardPattern(EntRef lhs, Wildcard rhs) {
  this->lhs = std::move(lhs);
  this->rhs = rhs;
}
ExactPattern::ExactPattern(EntRef lhs, std::string rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}
PartialPattern::PartialPattern(EntRef lhs, std::string rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}
