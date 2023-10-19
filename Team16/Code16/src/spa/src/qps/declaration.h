#pragma once

#include <string>
#include <functional>

#include "utils/entity_types.h"
#include "qps/design_entity.h"

struct Declaration {
  std::string synonym;
  DesignEntity design_entity;

  /*!
   * Checks if a declaration is equal to another declaration
   * @param other declaration to compare
   * @return true if the declarations are equal, else false
   */
  bool operator==(const Declaration& other) const {
    return synonym == other.synonym && design_entity == other.design_entity;
  }
};

/*!
 * Hash function for Declaration
 */
namespace std {
template<>
struct hash<Declaration> {
  size_t operator()(const Declaration& decl) const {
    size_t h1 = std::hash<std::string>()(decl.synonym);
    size_t h2 = std::hash<int>()(static_cast<int>(decl.design_entity));
    return h1 ^ (h2 << 1);
  }
};
}
