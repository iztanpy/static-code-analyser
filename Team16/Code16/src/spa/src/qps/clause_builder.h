#pragma once

#include <string>
#include <utility>
#include "qps/clause.h"

class ClauseBuilder {
 protected:
    virtual Clause& getClause() = 0;
    /* TODO: add variant of such that and pattern clauses */
};
