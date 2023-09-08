#pragma once

#include <vector>
#include "qps/parsed_query.h"
#include "qps/query_tokenizer.h"
#include "qps/select_clause.h"
#include "qps/declaration.h"


class QueryParser {
 public:
    static ParsedQuery ParseTokenizedQuery(const std::vector<QueryToken>& tokens);
    static std::vector<Declaration> ExtractDeclarations(const std::vector<QueryToken>& tokens);
    static std::vector<SelectClause>
    ExtractSelectClauses(const std::vector<QueryToken>& selectTokens, const std::vector<Declaration>& declarations);
    static std::vector<QueryToken> ExtractSelectTokens(const std::vector<QueryToken> &tokens);
};
