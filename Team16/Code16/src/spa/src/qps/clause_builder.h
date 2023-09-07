//
// Created by Thia Su Mian on 5/9/23.
//
#pragma once

#include <string>
#include <utility>

class ClauseBuilder {
 protected:
    virtual Clause& getClause() = 0;
    /* TODO: add variant of such that and pattern clauses */
};
