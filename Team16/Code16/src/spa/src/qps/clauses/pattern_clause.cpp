#pragma once

#include <string>
#include <utility>
#include "qps/clauses/pattern_clause.h"

WildCardPattern::WildCardPattern(Declaration syn_assignment, EntRef lhs, Wildcard rhs) {
  this->syn_assignment = std::move(syn_assignment);
  this->lhs = std::move(lhs);
  this->rhs = rhs;
}
ExactPattern::ExactPattern(Declaration syn_assignment, EntRef lhs, std::string rhs) {
  this->syn_assignment = std::move(syn_assignment);
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}
PartialPattern::PartialPattern(Declaration syn_assignment, EntRef lhs, std::string rhs) {
  this->syn_assignment = std::move(syn_assignment);
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}
bool PatternClause::are_expr_spec_equal(ExprSpec expr_1, ExprSpec expr_2) {
  if (std::holds_alternative<std::string>(expr_1) && std::holds_alternative<std::string>(expr_2)) {
    return std::get<std::string>(expr_1) == std::get<std::string>(expr_2);
  } else if (std::holds_alternative<Wildcard>(expr_1) && std::holds_alternative<Wildcard>(expr_2)) {
    return true;
  } else {
    return false;
  }
}
