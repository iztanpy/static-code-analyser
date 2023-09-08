#pragma once

#include "qps/select_clause.h"

struct ParsedQuery {
    SelectClause select;
    /* TODO: such that and pattern clauses */
};
