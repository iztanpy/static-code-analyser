#pragma once

#include <string>
#include "qps/design_entity.h"
#include "qps/clause.h"

class SelectClause: public Clause {
 public:
    DesignEntity design_entity;
    std::string synonym;
};
