#pragma once

#include <string>

#include "qps/design_entity.h"

struct Declaration {
  std::string synonym;
  DesignEntity design_entity;

  /*!
   * Checks if a declaration is equal to another declaration
   * @param other declaration to compare
   * @return true if the declarations are equal, else false
   */
  bool equals(Declaration & other) const;
};
