#pragma once

#include <stdio.h>
#include <string>
#include "SelectClause.h"
#include "SelectClauseBuilder.h"

class ClauseDirector {
 public:
    static bool dummyMethod();
    void makeSelectClause(SelectClauseBuilder builder, std::string query);
};
