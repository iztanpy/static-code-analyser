//
// Created by Cuong Ngo on 7/9/23.
//
#pragma once

#include <string>

#include "ParsedQuery.h"

class ClauseBuilder {
 public:
  ParsedQuery build(std::string query);
};
