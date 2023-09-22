#pragma once

#include <string>

#include "qps/design_entity.h"

struct Declaration {
  std::string synonym;
  DesignEntity design_entity;
  bool equals(Declaration & other) const;
};
