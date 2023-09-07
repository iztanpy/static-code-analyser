#pragma once

#include <stdio.h>
#include <string>

class SelectClause {
 public:
    std::string declaration;
    std::string keyword;
    std::string synonym;
    /* TODO: include such-that and pattern clauses */
};
