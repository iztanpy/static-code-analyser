#pragma once

#include <string>
#include <set>
#include "utils/StmtEntity.h"

class Entity {
 public:
  /*!
   * Converts a design entity to string
   * @param entity the design entity
   * @return a string representation of the design entity
   */
  static std::string toString(DesignEntity entity);

  /*!
   * Converts a string to a design entity
   * @param entity_str the string to convert
   * @return a design entity representation of the string
   */
  static DesignEntity fromString(const std::string& entity_str);

  /*!
   * Returns a set of design entities in string representation
   * @return the set of design entities in string representation
   */
  static std::set<std::string> getStringDesignEntities();
};
