//
// Created by Thia Su Mian on 4/9/23.
//

#pragma once

#include <stdio.h>
#include <string>
#include "SelectClause.h"
#include "SelectClauseBuilder.h"

class ClauseDirector {
 public:
    static bool dummyMethod();
    SelectClause makeSelectClause(SelectClauseBuilder builder, std::string query);
};
