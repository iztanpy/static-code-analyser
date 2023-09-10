#pragma once

#include <string>
#include <vector>
#include "qps/select_clause.h"
#include "qps/select_clause_builder.h"
#include "qps/query_tokenizer.h"
#include "qps/declaration.h"

class ClauseDirector {
 public:
    static bool dummyMethod();
    static SelectClause makeSelectClause(
            SelectClauseBuilder builder, const QueryToken& token, const std::vector<Declaration>& declarations);
};
