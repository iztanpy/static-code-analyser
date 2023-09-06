//
// Created by Thia Su Mian on 5/9/23.
//
#pragma once

#include <string>
#include <utility>

class ClauseBuilder {
 protected:
    std::string keyword;

 public:
    // pure virtual function
    virtual bool validateClause() = 0;
    void setKeyword(std::string kw);
};
