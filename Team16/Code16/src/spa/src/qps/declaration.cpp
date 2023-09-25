#include "qps/declaration.h"

bool Declaration::equals(Declaration other) const {
  return this->synonym == other.synonym && this->design_entity == other.design_entity;
}
