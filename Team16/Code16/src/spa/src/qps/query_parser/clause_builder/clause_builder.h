#pragma once

#include <string>
#include <utility>
#include "qps/clauses/clause.h"

class ClauseBuilder {
 protected:
  ClauseBuilder() = default;
  virtual ~ClauseBuilder() = default;
};
