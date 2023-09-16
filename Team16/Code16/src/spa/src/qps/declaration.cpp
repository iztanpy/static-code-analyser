#include "qps/declaration.h"
bool Declaration::equals(Declaration & other) {
  return this->synonym == other.synonym && this->design_entity == other.design_entity;
}
