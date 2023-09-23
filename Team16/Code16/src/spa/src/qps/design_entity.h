#pragma once

#include <string>
#include <set>
#include "utils/StmtEntity.h"

class Entity {
 public:
  static std::string toString(DesignEntity entity);
  static DesignEntity fromString(const std::string& entity_str);
  static std::set<std::string> getStringDesignEntities();
};
