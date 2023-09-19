#pragma once

#include <string>
#include <utility>
#include "qps/clauses/pattern_clause.h"

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
